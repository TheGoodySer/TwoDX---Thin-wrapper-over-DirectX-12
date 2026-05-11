#pragma once
#include <DXGInfrastructure/DXGInfrastructure.h>
#include <CommandQueue/D3D12CommandQueue.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <memory>

namespace TwoDX
{
	class DXGISwapChain
	{
	public:
		DXGISwapChain(
			DXGInfrastructure* pDXGI,
			D3D12CommandQueue* pCmdQueue,
			HWND hWnd,
			UINT width,
			UINT height,
			UINT bufferCount = 2
		);
		~DXGISwapChain();

		IDXGISwapChain4* GetSwapChain() const;
		UINT GetCurrentBackBufferIndex() const;

		void Present(bool vSyncEnable = true);
		void GetBuffer(UINT index, Microsoft::WRL::ComPtr<ID3D12Resource>& pBackBuffer);

	private:
		struct Impl;

		std::unique_ptr<Impl> pImpl = nullptr;
	};
}
