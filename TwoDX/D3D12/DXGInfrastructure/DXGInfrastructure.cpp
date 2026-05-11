#include <DXGInfrastructure/DXGInfrastructure.h>
#include <wrl/client.h>
#include <d3d12.h>

struct TwoDX::DXGInfrastructure::Impl
{
	Microsoft::WRL::ComPtr<IDXGIFactory6> pFactory = nullptr;
	Microsoft::WRL::ComPtr<IDXGIAdapter4> pAdapter = nullptr;
};

TwoDX::DXGInfrastructure::DXGInfrastructure()
{
	pImpl = std::make_unique<Impl>();

#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3D12Debug> pDebugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pDebugController))))
		pDebugController->EnableDebugLayer();
#endif

	UINT dxgiFactoryFlags = 0;
#ifdef _DEBUG
	dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	HRESULT hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&pImpl->pFactory));
	if (FAILED(hr)) throw std::exception("Failed to create factory.");

	for (UINT i = 0; pImpl->pFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&pImpl->pAdapter)) !=
		DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_ADAPTER_DESC3 desc = {};
		pImpl->pAdapter->GetDesc3(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			continue;

		break;
	}

	if (!pImpl->pAdapter) throw std::exception("Failed to create adapter.");
}

TwoDX::DXGInfrastructure::~DXGInfrastructure() = default;

IDXGIFactory6* TwoDX::DXGInfrastructure::GetFactory() const
{
	return pImpl->pFactory.Get();
}

IDXGIAdapter4* TwoDX::DXGInfrastructure::GetAdapter() const
{
	return pImpl->pAdapter.Get();
}
