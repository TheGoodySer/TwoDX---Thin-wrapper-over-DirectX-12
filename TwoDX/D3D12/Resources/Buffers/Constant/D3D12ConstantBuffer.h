#pragma once
#include <Device/D3D12Device.h>
#include <memory>

namespace TwoDX
{
	class D3D12ConstantBuffer
	{
	public:
		D3D12ConstantBuffer(
			D3D12Device* pD3D12Device,
			size_t srcSize
		);
		~D3D12ConstantBuffer();

		void UpdateBuffer(void* pSrc);

		D3D12_GPU_VIRTUAL_ADDRESS GetVirtualAddress() const;

	private:
		struct Impl;

		std::unique_ptr<Impl> pImpl = nullptr;
	};
}
