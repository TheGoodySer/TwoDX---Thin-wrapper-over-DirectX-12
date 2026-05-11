#pragma once
#include <Device/D3D12Device.h>
#include <CommandQueue/D3D12CommandQueue.h>
#include <memory>
#include <d3d12.h>
#include <Synchronizer/D3D12Synchronizer.h>
#include <DirectXMath.h>
#include <Structures/TwoDXStructs.h>

namespace TwoDX
{
	class D3D12VertexBuffer
	{
	public:
		D3D12VertexBuffer(
			D3D12Device* pD3D12Device,
			D3D12CommandQueue* pCmdQueue,
			D3D12Synchronizer* pSync,
			void* pSrc,
			size_t srcSize
		);
		~D3D12VertexBuffer();

		ID3D12Resource* GetBuffer() const;
		D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferView();

	private:
		struct Impl;

		std::unique_ptr<Impl> pImpl = nullptr;
	};
}
