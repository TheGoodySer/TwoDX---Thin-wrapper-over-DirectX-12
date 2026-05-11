#include <Device/D3D12Device.h>
#include <wrl/client.h>

struct TwoDX::D3D12Device::Impl
{
	Microsoft::WRL::ComPtr<ID3D12Device6> pD3D12Device = nullptr;
};

TwoDX::D3D12Device::D3D12Device(DXGInfrastructure* pDXGI)
{
	pImpl = std::make_unique<Impl>();

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_2,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0
	};

	for (const auto& level : featureLevels)
	{
		if (SUCCEEDED(D3D12CreateDevice(pDXGI->GetAdapter(), level, IID_PPV_ARGS(&pImpl->pD3D12Device))))
			break;
	}

	if (!pImpl->pD3D12Device) throw std::exception("Failed to create device.");
}

TwoDX::D3D12Device::~D3D12Device() = default;

ID3D12Device6* TwoDX::D3D12Device::GetDevice() const
{
	return pImpl->pD3D12Device.Get();
}
