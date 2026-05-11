#include <Resources/Texture/Texture2D.h>
#include <wrl/client.h>
#include <WICTextureLoader.h>
#include <ResourceUploadBatch.h>
#include <d3dx12.h>

struct TwoDX::Texture2D::Impl
{
	Microsoft::WRL::ComPtr<ID3D12Resource> pTexture = nullptr;
};

TwoDX::Texture2D::Texture2D(
	D3D12Device* pD3D12Device,
	D3D12CommandQueue* pCmdQueue,
	const std::wstring& texPath
)
{
	pImpl = std::make_unique<Impl>();

	DirectX::ResourceUploadBatch batch(pD3D12Device->GetDevice());
	batch.Begin();

	HRESULT hr = DirectX::CreateWICTextureFromFile(pD3D12Device->GetDevice(), batch, texPath.c_str(), &pImpl->pTexture, true);

	auto end = batch.End(pCmdQueue->GetCommandQueue());
	end.wait();
}

TwoDX::Texture2D::~Texture2D() = default;

ID3D12Resource* TwoDX::Texture2D::GetResource() const
{
	return pImpl->pTexture.Get();
}

const D3D12_RESOURCE_DESC& TwoDX::Texture2D::GetDesc() const
{
	return pImpl->pTexture->GetDesc();
}
