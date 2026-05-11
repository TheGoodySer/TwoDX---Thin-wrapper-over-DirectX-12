#include <CommandQueue/D3D12CommandQueue.h>
#include <wrl/client.h>

struct TwoDX::D3D12CommandQueue::Impl
{
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> pCommandQueue = nullptr;
};

TwoDX::D3D12CommandQueue::D3D12CommandQueue(D3D12Device* pD3D12Device, D3D12_COMMAND_LIST_TYPE type, D3D12_COMMAND_QUEUE_PRIORITY priority)
{
	pImpl = std::make_unique<Impl>();

	ID3D12Device* pDevice = pD3D12Device->GetDevice();

	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = type;
	desc.Priority = priority;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	HRESULT hr = pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&pImpl->pCommandQueue));
	if (FAILED(hr)) throw std::exception("Failed to create command queue.");
}

TwoDX::D3D12CommandQueue::~D3D12CommandQueue() = default;

ID3D12CommandQueue* TwoDX::D3D12CommandQueue::GetCommandQueue() const
{
	return pImpl->pCommandQueue.Get();
}

void TwoDX::D3D12CommandQueue::Signal(ID3D12Fence* pFence, UINT64 fenceValue)
{
	pImpl->pCommandQueue->Signal(pFence, fenceValue);
}

void TwoDX::D3D12CommandQueue::ExecuteCommandLists(UINT listCount, ID3D12CommandList** lists)
{
	pImpl->pCommandQueue->ExecuteCommandLists(listCount, lists);
}
