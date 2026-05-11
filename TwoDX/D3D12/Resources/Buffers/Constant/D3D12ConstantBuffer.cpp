#include <Resources/Buffers/Constant/D3D12ConstantBuffer.h>
#include <wrl/client.h>

struct TwoDX::D3D12ConstantBuffer::Impl
{
	Microsoft::WRL::ComPtr<ID3D12Resource> pConstBuffer = nullptr;
	void* pCBMapped = nullptr;
	size_t dataSize = 0;
};

TwoDX::D3D12ConstantBuffer::D3D12ConstantBuffer(
	D3D12Device* pD3D12Device,
	size_t srcSize
)
{
	pImpl = std::make_unique<Impl>();
	pImpl->dataSize = srcSize;

	ID3D12Device6* pDevice = pD3D12Device->GetDevice();

	CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(srcSize);
	CD3DX12_HEAP_PROPERTIES props(D3D12_HEAP_TYPE_UPLOAD);

	HRESULT hr = pDevice->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&pImpl->pConstBuffer));
	if (FAILED(hr)) throw std::exception("Failed to create constant buffer.");

	pImpl->pConstBuffer->Map(0, nullptr, &pImpl->pCBMapped);
}

TwoDX::D3D12ConstantBuffer::~D3D12ConstantBuffer()
{
	pImpl->pConstBuffer->Unmap(0, nullptr);
}

void TwoDX::D3D12ConstantBuffer::UpdateBuffer(void* pSrc)
{
	if (!pSrc) return;

	memcpy(pImpl->pCBMapped, pSrc, pImpl->dataSize);
}

D3D12_GPU_VIRTUAL_ADDRESS TwoDX::D3D12ConstantBuffer::GetVirtualAddress() const
{
	return pImpl->pConstBuffer->GetGPUVirtualAddress();
}
