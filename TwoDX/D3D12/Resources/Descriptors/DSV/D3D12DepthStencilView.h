#pragma once
#include <Device/D3D12Device.h>
#include <memory>

namespace TwoDX
{
	class D3D12DepthStencilView
	{
	public:
		D3D12DepthStencilView(
			D3D12Device* pD3D12Device,
			UINT width,
			UINT height
		);
		~D3D12DepthStencilView();

		ID3D12DescriptorHeap* GetDSVHeap() const;
		ID3D12Resource* GetDepthBuffer() const;

		D3D12_CPU_DESCRIPTOR_HANDLE GetIncrementDescriptorHeapFromStart();

	private:
		struct Impl;

		std::unique_ptr<Impl> pImpl = nullptr;
	};
}
