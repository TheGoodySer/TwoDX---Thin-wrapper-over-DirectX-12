#pragma once
#include <Device/D3D12Device.h>
#include <Synchronizer/D3D12Synchronizer.h>
#include <CommandQueue/D3D12CommandQueue.h>
#include <memory>

namespace TwoDX
{
	class D3D12IndexBuffer
	{
	public:
		D3D12IndexBuffer(
			D3D12Device* pD3D12Device,
			D3D12CommandQueue* pCmdQueue,
			D3D12Synchronizer* pSync,
			uint32_t* pSrc,
			size_t srcSize
		);
		~D3D12IndexBuffer();

		ID3D12Resource* GetBuffer() const;
		D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView();


	private:
		struct Impl;

		std::unique_ptr<Impl> pImpl = nullptr;
	};
}
