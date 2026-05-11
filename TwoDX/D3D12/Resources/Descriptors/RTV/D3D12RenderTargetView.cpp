#include <Resources/Descriptors/RTV/D3D12RenderTargetView.h>
#include <wrl/client.h>

struct TwoDX::D3D12RenderTargetView::Impl
{
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> pRTVHeap = nullptr;
	UINT rtvDescriptorSize = 0;
};

TwoDX::D3D12RenderTargetView::D3D12RenderTargetView(D3D12Device* pD3D12Device, DXGISwapChain* pSwapChain, UINT rtvCount)
{
	pImpl = std::make_unique<Impl>();

	ID3D12Device6* pDevice = pD3D12Device->GetDevice();

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.NumDescriptors = rtvCount;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask = 0;

	HRESULT hr = pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&pImpl->pRTVHeap));
	if (FAILED(hr)) throw std::exception("Failed to create RTV heap.");

	pImpl->rtvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	auto handle = pImpl->pRTVHeap->GetCPUDescriptorHandleForHeapStart();

	for (UINT i = 0; i < rtvCount; i++)
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> pBackBuffer = nullptr;
		pSwapChain->GetBuffer(i, pBackBuffer);

		pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, handle);
		handle.ptr += pImpl->rtvDescriptorSize;
	}
}

TwoDX::D3D12RenderTargetView::~D3D12RenderTargetView() = default;

ID3D12DescriptorHeap* TwoDX::D3D12RenderTargetView::GetRTVHeap() const
{
	return pImpl->pRTVHeap.Get();
}

UINT64 TwoDX::D3D12RenderTargetView::GetRTVDescSize() const
{
	return pImpl->rtvDescriptorSize;
}

D3D12_CPU_DESCRIPTOR_HANDLE TwoDX::D3D12RenderTargetView::GetIncrementDescriptorHeapFromStart()
{
	return pImpl->pRTVHeap->GetCPUDescriptorHandleForHeapStart();
}
