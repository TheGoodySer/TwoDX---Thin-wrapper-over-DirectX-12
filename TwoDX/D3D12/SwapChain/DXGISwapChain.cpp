#include <SwapChain/DXGISwapChain.h>
#include <wrl/client.h>
#include <dxgi1_6.h>

struct TwoDX::DXGISwapChain::Impl
{
	Microsoft::WRL::ComPtr<IDXGISwapChain4> pSwapChain = nullptr;
};

TwoDX::DXGISwapChain::DXGISwapChain(
	DXGInfrastructure* pDXGI,
	D3D12CommandQueue* pCmdQueue,
	HWND hWnd,
	UINT width,
	UINT height,
	UINT bufferCount
)
{
	pImpl = std::make_unique<Impl>();

	IDXGIFactory6* pFactory = pDXGI->GetFactory();

	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferCount = bufferCount;
	desc.SampleDesc.Count = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Scaling = DXGI_SCALING_STRETCH;
	desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.Flags = 0;

	Microsoft::WRL::ComPtr<IDXGISwapChain1> pTempSC = nullptr;

	HRESULT hr = pFactory->CreateSwapChainForHwnd(pCmdQueue->GetCommandQueue(), hWnd, &desc, nullptr, nullptr, &pTempSC);
	if (FAILED(hr)) throw std::exception("Failed to create swap chain.");

	hr = pTempSC->QueryInterface(IID_PPV_ARGS(&pImpl->pSwapChain));
	if (FAILED(hr)) throw std::exception("Failed to get swap chain interface.");
}

TwoDX::DXGISwapChain::~DXGISwapChain() = default;

IDXGISwapChain4* TwoDX::DXGISwapChain::GetSwapChain() const
{
	return pImpl->pSwapChain.Get();
}

UINT TwoDX::DXGISwapChain::GetCurrentBackBufferIndex() const
{
	return pImpl->pSwapChain->GetCurrentBackBufferIndex();
}

void TwoDX::DXGISwapChain::Present(bool vSyncEnable)
{
	pImpl->pSwapChain->Present((UINT)vSyncEnable, 0);
}

void TwoDX::DXGISwapChain::GetBuffer(UINT index, Microsoft::WRL::ComPtr<ID3D12Resource>& pBackBuffer)
{
	pImpl->pSwapChain->GetBuffer(index, IID_PPV_ARGS(&pBackBuffer));
}
