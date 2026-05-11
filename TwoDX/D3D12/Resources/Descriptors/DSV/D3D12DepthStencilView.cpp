#include <Resources/Descriptors/DSV/D3D12DepthStencilView.h>
#include <d3dx12.h>
#include <wrl/client.h>

struct TwoDX::D3D12DepthStencilView::Impl
{
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> pDSVHeap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> pDepthBuffer = nullptr;
};

TwoDX::D3D12DepthStencilView::D3D12DepthStencilView(
	D3D12Device* pD3D12Device,
	UINT width,
	UINT height
)
{
	pImpl = std::make_unique<Impl>();

	ID3D12Device6* pDevice = pD3D12Device->GetDevice();

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	desc.NumDescriptors = 1;
	desc.NodeMask = 0;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HRESULT hr = pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&pImpl->pDSVHeap));
	if (FAILED(hr)) throw std::exception("Failed to create dsv heap.");

	CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D24_UNORM_S8_UINT, width, height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	CD3DX12_HEAP_PROPERTIES props(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_CLEAR_VALUE clearVal = {};
	clearVal.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	clearVal.DepthStencil.Depth = 1.0f;

	hr = pDevice->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearVal, IID_PPV_ARGS(&pImpl->pDepthBuffer));
	if (FAILED(hr)) throw std::exception("Failed to create depth buffer.");

	pDevice->CreateDepthStencilView(pImpl->pDepthBuffer.Get(), nullptr, pImpl->pDSVHeap->GetCPUDescriptorHandleForHeapStart());
}

TwoDX::D3D12DepthStencilView::~D3D12DepthStencilView()
{
}

ID3D12DescriptorHeap* TwoDX::D3D12DepthStencilView::GetDSVHeap() const
{
	return pImpl->pDSVHeap.Get();
}

ID3D12Resource* TwoDX::D3D12DepthStencilView::GetDepthBuffer() const
{
	return pImpl->pDepthBuffer.Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE TwoDX::D3D12DepthStencilView::GetIncrementDescriptorHeapFromStart()
{
	return pImpl->pDSVHeap->GetCPUDescriptorHandleForHeapStart();
}
