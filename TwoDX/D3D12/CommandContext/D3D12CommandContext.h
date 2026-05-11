#pragma once
#include <Device/D3D12Device.h>
#include <Resources/Descriptors/DSV/D3D12DepthStencilView.h>
#include <d3d12.h>
#include <memory>

namespace TwoDX
{
	class D3D12CommandContext
	{
	public:
		D3D12CommandContext(D3D12Device* pD3D12Device, D3D12_COMMAND_LIST_TYPE type);
		~D3D12CommandContext();

		ID3D12CommandAllocator* GetCommandAllocator() const;
		ID3D12GraphicsCommandList* GetCommandList() const;

		void Begin(ID3D12PipelineState* pPSO = nullptr);
		void SetRTV(UINT numDescriptors, D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle);
		void SetVP(UINT numVP, D3D12_VIEWPORT* vp);
		void SetSR(UINT numSR, D3D12_RECT* rc);
		void SetGraphicsRootSignature(ID3D12RootSignature* pRootSignature);
		void SetVertexBuffers(UINT numViews, D3D12_VERTEX_BUFFER_VIEW* vbv);
		void SetIndexBuffer(D3D12_INDEX_BUFFER_VIEW* ibv);
		void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology);
		void SetGraphicsRootSignatureConstBufferView(UINT paramIndex, D3D12_GPU_VIRTUAL_ADDRESS bufferLocation);
		void SetGraphicsRootSignatureShaderResourceView(UINT paramIndex, D3D12_GPU_VIRTUAL_ADDRESS bufferLocation);
		void SetDescriptorHeaps(UINT heapCount, ID3D12DescriptorHeap** ppHeaps);
		void SetGraphicsRootSignatureDescriptorTable(UINT paramIndex, D3D12_GPU_DESCRIPTOR_HANDLE handle);
		void ClearRTV(D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle, float red, float green, float blue, float alpha);
		void ClearDSV(D3D12DepthStencilView* pDSV);
		void ResourceBarrier(UINT barrierCount, D3D12_RESOURCE_BARRIER* barriers);
		void DrawInstanced(UINT vertices, UINT instances);
		void DrawIndexedInstanced(UINT indices, UINT instances);
		void End();

	private:
		struct Impl;

		std::unique_ptr<Impl> pImpl = nullptr;
	};
}
