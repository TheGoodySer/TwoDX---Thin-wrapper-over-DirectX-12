#pragma once
#include <Device/D3D12Device.h>
#include <d3d12.h>
#include <memory>

namespace TwoDX
{
	class D3D12CommandQueue
	{
	public:
		D3D12CommandQueue(D3D12Device* pD3D12Device, D3D12_COMMAND_LIST_TYPE type, D3D12_COMMAND_QUEUE_PRIORITY priority);
		~D3D12CommandQueue();

		ID3D12CommandQueue* GetCommandQueue() const;

		void Signal(ID3D12Fence* pFence, UINT64 fenceValue);
		void ExecuteCommandLists(UINT listCount, ID3D12CommandList** lists);

	private:
		struct Impl;

		std::unique_ptr<Impl> pImpl = nullptr;
	};
}
