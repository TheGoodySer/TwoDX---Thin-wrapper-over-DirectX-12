#include <CommandContext/D3D12CommandContext.h>
#include <wrl/client.h>

struct TwoDX::D3D12CommandContext::Impl
{
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> pCmdAlloc = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList = nullptr;

	bool isRecording = false;
};

TwoDX::D3D12CommandContext::D3D12CommandContext(D3D12Device* pD3D12Device, D3D12_COMMAND_LIST_TYPE type)
{
	pImpl = std::make_unique<Impl>();

	ID3D12Device6* pDevice = pD3D12Device->GetDevice();

	HRESULT hr = pDevice->CreateCommandAllocator(type, IID_PPV_ARGS(&pImpl->pCmdAlloc));
	if (FAILED(hr)) throw std::exception("Failed to create allocator.");

	hr = pDevice->CreateCommandList(0, type, pImpl->pCmdAlloc.Get(), nullptr, IID_PPV_ARGS(&pImpl->pCmdList));
	if (FAILED(hr)) throw std::exception("Failed to create command list.");

	pImpl->pCmdList->Close();
}

TwoDX::D3D12CommandContext::~D3D12CommandContext() = default;

ID3D12CommandAllocator* TwoDX::D3D12CommandContext::GetCommandAllocator() const
{
	return pImpl->pCmdAlloc.Get();
}

ID3D12GraphicsCommandList* TwoDX::D3D12CommandContext::GetCommandList() const
{
	return pImpl->pCmdList.Get();
}

void TwoDX::D3D12CommandContext::Begin(ID3D12PipelineState* pPSO)
{
	if (pImpl->isRecording) return;

	pImpl->pCmdAlloc->Reset();
	pImpl->pCmdList->Reset(pImpl->pCmdAlloc.Get(), pPSO);

	pImpl->isRecording = true;
}

void TwoDX::D3D12CommandContext::SetRTV(UINT numDescriptors, D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle)
{
	if (!pImpl->isRecording) return;

	pImpl->pCmdList->OMSetRenderTargets(numDescriptors, rtvHandle, FALSE, dsvHandle);
}

void TwoDX::D3D12CommandContext::SetVP(UINT numVP, D3D12_VIEWPORT* vp)
{
	if (!pImpl->isRecording) return;

	pImpl->pCmdList->RSSetViewports(numVP, vp);
}

void TwoDX::D3D12CommandContext::SetSR(UINT numSR, D3D12_RECT* rc)
{
	if (!pImpl->isRecording) return;

	pImpl->pCmdList->RSSetScissorRects(numSR, rc);
}

void TwoDX::D3D12CommandContext::SetGraphicsRootSignature(ID3D12RootSignature* pRootSignature)
{
	if (!pImpl->isRecording) return;

	pImpl->pCmdList->SetGraphicsRootSignature(pRootSignature);
}

void TwoDX::D3D12CommandContext::SetVertexBuffers(UINT numViews, D3D12_VERTEX_BUFFER_VIEW* vbv)
{
	if (!pImpl->isRecording) return;

	pImpl->pCmdList->IASetVertexBuffers(0, numViews, vbv);
}

void TwoDX::D3D12CommandContext::SetIndexBuffer(D3D12_INDEX_BUFFER_VIEW* ibv)
{
	if (!pImpl->isRecording) return;

	pImpl->pCmdList->IASetIndexBuffer(ibv);
}

void TwoDX::D3D12CommandContext::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
	if (!pImpl->isRecording) return;

	pImpl->pCmdList->IASetPrimitiveTopology(topology);
}

void TwoDX::D3D12CommandContext::SetGraphicsRootSignatureConstBufferView(UINT paramIndex, D3D12_GPU_VIRTUAL_ADDRESS bufferLocation)
{
	if (!pImpl->isRecording) return;

	pImpl->pCmdList->SetGraphicsRootConstantBufferView(paramIndex, bufferLocation);
}

void TwoDX::D3D12CommandContext::SetGraphicsRootSignatureShaderResourceView(UINT paramIndex, D3D12_GPU_VIRTUAL_ADDRESS bufferLocation)
{
	if (!pImpl->isRecording) return;

	pImpl->pCmdList->SetGraphicsRootShaderResourceView(paramIndex, bufferLocation);
}

void TwoDX::D3D12CommandContext::SetDescriptorHeaps(UINT heapCount, ID3D12DescriptorHeap** ppHeaps)
{
	if (!pImpl->isRecording) return;

	pImpl->pCmdList->SetDescriptorHeaps(heapCount, ppHeaps);
}

void TwoDX::D3D12CommandContext::SetGraphicsRootSignatureDescriptorTable(UINT paramIndex, D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
	if (!pImpl->isRecording) return;

	pImpl->pCmdList->SetGraphicsRootDescriptorTable(paramIndex, handle);
}

void TwoDX::D3D12CommandContext::ClearRTV(D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle, float red, float green, float blue, float alpha)
{
	if (!pImpl->isRecording) return;

	float rgba[] = { red, green, blue, alpha };
	pImpl->pCmdList->ClearRenderTargetView(*rtvHandle, rgba, 0, nullptr);
}

void TwoDX::D3D12CommandContext::ClearDSV(D3D12DepthStencilView* pDSV)
{
	pImpl->pCmdList->ClearDepthStencilView(pDSV->GetIncrementDescriptorHeapFromStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void TwoDX::D3D12CommandContext::ResourceBarrier(UINT barrierCount, D3D12_RESOURCE_BARRIER* barriers)
{
	if (!pImpl->isRecording) return;

	pImpl->pCmdList->ResourceBarrier(barrierCount, barriers);
}

void TwoDX::D3D12CommandContext::DrawInstanced(UINT vertexCountPerInstance, UINT instances)
{
	if (!pImpl->isRecording) return;

	pImpl->pCmdList->DrawInstanced(vertexCountPerInstance, instances, 0, 0);
}

void TwoDX::D3D12CommandContext::DrawIndexedInstanced(UINT indexCountPerInstance, UINT instances)
{
	if (!pImpl->isRecording) return;

	pImpl->pCmdList->DrawIndexedInstanced(indexCountPerInstance, instances, 0, 0, 0);
}

void TwoDX::D3D12CommandContext::End()
{
	if (!pImpl->isRecording) return;

	pImpl->pCmdList->Close();
	pImpl->isRecording = false;
}
