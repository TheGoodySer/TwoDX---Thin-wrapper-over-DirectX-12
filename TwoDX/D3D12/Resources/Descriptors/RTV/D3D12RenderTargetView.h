#pragma once
#include <Device/D3D12Device.h>
#include <SwapChain/DXGISwapChain.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>

namespace TwoDX
{
	class D3D12RenderTargetView
	{
	public:
		D3D12RenderTargetView(D3D12Device* pD3D12Device, DXGISwapChain* pSwapChain, UINT rtvCount);
		~D3D12RenderTargetView();

		ID3D12DescriptorHeap* GetRTVHeap() const;
		UINT64 GetRTVDescSize() const;

		D3D12_CPU_DESCRIPTOR_HANDLE GetIncrementDescriptorHeapFromStart();

	private:
		struct Impl;

		std::unique_ptr<Impl> pImpl = nullptr;
	};
}
