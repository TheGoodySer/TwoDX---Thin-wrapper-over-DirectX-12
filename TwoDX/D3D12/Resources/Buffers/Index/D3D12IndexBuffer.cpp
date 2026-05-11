#include <Resources/Buffers/Index/D3D12IndexBuffer.h>
#include <d3dx12.h>
#include <wrl/client.h>

struct TwoDX::D3D12IndexBuffer::Impl
{
	Microsoft::WRL::ComPtr<ID3D12Resource> pGPUBuffer = nullptr;
    D3D12_INDEX_BUFFER_VIEW ibv = {};
};

TwoDX::D3D12IndexBuffer::D3D12IndexBuffer(
	D3D12Device* pD3D12Device,
	D3D12CommandQueue* pCmdQueue,
	D3D12Synchronizer* pSync,
	uint32_t* pSrc,
	size_t srcSize
)
{
	pImpl = std::make_unique<Impl>();

    ID3D12Device6* pDevice = pD3D12Device->GetDevice();

    CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(srcSize);
    CD3DX12_HEAP_PROPERTIES gpuProps(D3D12_HEAP_TYPE_DEFAULT);

    HRESULT hr = pDevice->CreateCommittedResource(
        &gpuProps, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr, IID_PPV_ARGS(&pImpl->pGPUBuffer));
    if (FAILED(hr)) throw std::exception("Failed to create GPU Buffer.");

    Microsoft::WRL::ComPtr<ID3D12Resource> pCPUBuffer = nullptr;
    CD3DX12_HEAP_PROPERTIES cpuProps(D3D12_HEAP_TYPE_UPLOAD);

    hr = pDevice->CreateCommittedResource(
        &cpuProps, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr, IID_PPV_ARGS(&pCPUBuffer));
    if (FAILED(hr)) throw std::exception("Failed to create CPU Buffer.");

    void* pData;
    pCPUBuffer->Map(0, nullptr, &pData);
    memcpy(pData, pSrc, srcSize);
    pCPUBuffer->Unmap(0, nullptr);

    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> pCmdAlloc = nullptr;
    hr = pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCmdAlloc));
    if (FAILED(hr)) throw std::exception("Failed to create command allocator.");

    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList = nullptr;
    hr = pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCmdAlloc.Get(), nullptr, IID_PPV_ARGS(&pCmdList));
    if (FAILED(hr)) throw std::exception("Failed to create command list.");

    pCmdList->CopyBufferRegion(pImpl->pGPUBuffer.Get(), 0, pCPUBuffer.Get(), 0, srcSize);
    pCmdList->Close();

    ID3D12CommandList* lists[] = { pCmdList.Get() };
    pCmdQueue->ExecuteCommandLists(_countof(lists), lists);

    pSync->IncFenceValue();
    pCmdQueue->Signal(pSync->GetFence(), pSync->GetFenceValue());

    if (pSync->GetCompletedValue() < pSync->GetFenceValue())
    {
        HANDLE event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        pSync->SetEventOnCompletion(event);
        WaitForSingleObject(event, INFINITE);
        CloseHandle(event);
    }

    pImpl->ibv.BufferLocation = pImpl->pGPUBuffer->GetGPUVirtualAddress();
    pImpl->ibv.SizeInBytes = static_cast<UINT>(srcSize);
    pImpl->ibv.Format = DXGI_FORMAT_R32_UINT;
}

TwoDX::D3D12IndexBuffer::~D3D12IndexBuffer() = default;

ID3D12Resource* TwoDX::D3D12IndexBuffer::GetBuffer() const
{
    return pImpl->pGPUBuffer.Get();
}

D3D12_INDEX_BUFFER_VIEW* TwoDX::D3D12IndexBuffer::GetIndexBufferView()
{
    return &pImpl->ibv;
}


