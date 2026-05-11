#include <Resources/Descriptors/SRV/D3D12ShaderResourceView.h>
#include <wrl/client.h>
#include <d3dx12.h>

struct TwoDX::D3D12ShaderResourceView::Impl
{
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> pSRVHeap = nullptr;
};

TwoDX::D3D12ShaderResourceView::D3D12ShaderResourceView(
	D3D12Device* pD3D12Device,
	Texture2D* pTexture
)
{
	pImpl = std::make_unique<Impl>();

	auto pDevice = pD3D12Device->GetDevice();

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.NodeMask = 0;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	HRESULT hr = pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&pImpl->pSRVHeap));
	if (FAILED(hr)) throw std::exception("Failed to create SRV.");

	auto handle = pImpl->pSRVHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = pTexture->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = pTexture->GetDesc().MipLevels;

	pDevice->CreateShaderResourceView(pTexture->GetResource(), &srvDesc, handle);
}

TwoDX::D3D12ShaderResourceView::~D3D12ShaderResourceView() = default;

ID3D12DescriptorHeap* TwoDX::D3D12ShaderResourceView::GetDSVHeap() const
{
	return pImpl->pSRVHeap.Get();
}

D3D12_GPU_DESCRIPTOR_HANDLE TwoDX::D3D12ShaderResourceView::GetIncrementDescriptorHeapFromStart()
{
	return pImpl->pSRVHeap->GetGPUDescriptorHandleForHeapStart();
}
