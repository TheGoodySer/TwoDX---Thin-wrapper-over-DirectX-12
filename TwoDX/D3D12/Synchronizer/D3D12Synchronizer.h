#pragma once
#include <Device/D3D12Device.h>
#include <memory>
#include <d3d12.h>

namespace TwoDX
{
	class D3D12Synchronizer
	{
	public:
		D3D12Synchronizer(D3D12Device* pD3D12Device);
		~D3D12Synchronizer();

		ID3D12Fence* GetFence() const;
		UINT64 GetFenceValue() const;
		UINT64 GetCompletedValue() const;
			 
		void SetEventOnCompletion(HANDLE event);
		void IncFenceValue();

	private:
		struct Impl;

		std::unique_ptr<Impl> pImpl = nullptr;
	};
}
