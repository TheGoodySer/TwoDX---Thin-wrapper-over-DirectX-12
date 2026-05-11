#include <Synchronizer/D3D12Synchronizer.h>
#include <wrl/client.h>

struct TwoDX::D3D12Synchronizer::Impl
{
	Microsoft::WRL::ComPtr<ID3D12Fence> pFence = nullptr;
	UINT64 fenceValue = 0;
};

TwoDX::D3D12Synchronizer::D3D12Synchronizer(D3D12Device* pD3D12Device)
{
	pImpl = std::make_unique<Impl>();

	ID3D12Device6* pDevice = pD3D12Device->GetDevice();

	HRESULT hr = pDevice->CreateFence(pImpl->fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pImpl->pFence));
	if (FAILED(hr)) throw std::exception("Failed to create fence.");
}

TwoDX::D3D12Synchronizer::~D3D12Synchronizer() = default;

ID3D12Fence* TwoDX::D3D12Synchronizer::GetFence() const
{
	return pImpl->pFence.Get();
}

UINT64 TwoDX::D3D12Synchronizer::GetFenceValue() const
{
	return pImpl->fenceValue;
}

UINT64 TwoDX::D3D12Synchronizer::GetCompletedValue() const
{
	return pImpl->pFence->GetCompletedValue();
}

void TwoDX::D3D12Synchronizer::SetEventOnCompletion(HANDLE event)
{
	pImpl->pFence->SetEventOnCompletion(pImpl->fenceValue, event);
}

void TwoDX::D3D12Synchronizer::IncFenceValue()
{
	pImpl->fenceValue++;
}
