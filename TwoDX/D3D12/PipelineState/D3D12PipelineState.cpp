#include <PipelineState/D3D12PipelineState.h>
#include <d3dx12.h>

struct TwoDX::D3D12PipelineState::Impl
{
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pPSO = nullptr;
};

TwoDX::D3D12PipelineState::D3D12PipelineState(
	D3D12Device* pD3D12Device,
	D3D12RootSignature* pRootSignature,
	ComPtr<ID3DBlob>& pVS,
	ComPtr<ID3DBlob>& pPS,
	UINT numRTV,
	D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType,
	CD3DX12_RASTERIZER_DESC& rasterizer,
	CD3DX12_BLEND_DESC& blend,
	std::vector<D3D12_INPUT_ELEMENT_DESC>& elements,
	bool depthEnable,
	bool stencilEnable
)
{
	pImpl = std::make_unique<Impl>();

	ID3D12Device6* pDevice = pD3D12Device->GetDevice();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	desc.NodeMask = 0;
	desc.pRootSignature = pRootSignature->GetRootSignature();
	desc.VS = { pVS->GetBufferPointer(), pVS->GetBufferSize()};
	desc.PS = { pPS->GetBufferPointer(), pPS->GetBufferSize() };
	desc.DepthStencilState.DepthEnable = (BOOL)depthEnable;
	desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.DepthStencilState.StencilEnable = (BOOL)stencilEnable;
	desc.NumRenderTargets = numRTV;
	desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.PrimitiveTopologyType = topologyType;
	desc.SampleMask = UINT_MAX;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.RasterizerState = rasterizer;
	desc.BlendState = blend;
	desc.InputLayout.NumElements = elements.size();
	desc.InputLayout.pInputElementDescs = elements.data();

	HRESULT hr = pDevice->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pImpl->pPSO));
	if (FAILED(hr)) throw std::exception("Failed to create graphics pipeline state.");
}

TwoDX::D3D12PipelineState::~D3D12PipelineState() = default;

ID3D12PipelineState* TwoDX::D3D12PipelineState::GetPSO() const
{
	return pImpl->pPSO.Get();
}
