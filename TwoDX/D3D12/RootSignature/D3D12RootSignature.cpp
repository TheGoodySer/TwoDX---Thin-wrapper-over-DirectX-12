#include <RootSignature/D3D12RootSignature.h>
#include <d3dx12.h>
#include <wrl/client.h>

struct TwoDX::D3D12RootSignature::Impl
{
	Microsoft::WRL::ComPtr<ID3D12RootSignature> pRootSignature = nullptr;
};

TwoDX::D3D12RootSignature::D3D12RootSignature(
	D3D12Device* pD3D12Device,
	UINT numParams,
	CD3DX12_ROOT_PARAMETER* params,
	UINT numSamplers,
	CD3DX12_STATIC_SAMPLER_DESC* samplerDescs
)
{
	pImpl = std::make_unique<Impl>();

	ID3D12Device6* pDevice = pD3D12Device->GetDevice();

	CD3DX12_ROOT_SIGNATURE_DESC desc = CD3DX12_ROOT_SIGNATURE_DESC(numParams, params, numSamplers, samplerDescs, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob, pError = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &pBlob, &pError);
	if (FAILED(hr)) throw std::exception("Failed to serialize root signature.");

	hr = pDevice->CreateRootSignature(0, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), IID_PPV_ARGS(&pImpl->pRootSignature));
	if (FAILED(hr)) throw std::exception("Failed to create root signature.");
}

TwoDX::D3D12RootSignature::~D3D12RootSignature() = default;

ID3D12RootSignature* TwoDX::D3D12RootSignature::GetRootSignature() const
{
	return pImpl->pRootSignature.Get();
}
