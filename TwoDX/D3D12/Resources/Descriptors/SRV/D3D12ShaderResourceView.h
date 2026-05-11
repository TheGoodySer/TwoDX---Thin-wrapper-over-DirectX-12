#pragma once
#include <Device/D3D12Device.h>
#include <Resources/Texture/Texture2D.h>
#include <memory>

namespace TwoDX
{
	class D3D12ShaderResourceView
	{
	public:
		D3D12ShaderResourceView(
			D3D12Device* pD3D12Device,
			Texture2D* ppTexture
		);
		~D3D12ShaderResourceView();

		ID3D12DescriptorHeap* GetDSVHeap() const;

		D3D12_GPU_DESCRIPTOR_HANDLE GetIncrementDescriptorHeapFromStart();

	private:
		struct Impl;

		std::unique_ptr<Impl> pImpl = nullptr;
	};
}
