#pragma once
#include <Device/D3D12Device.h>
#include <RootSignature/D3D12RootSignature.h>
#include <memory>
#include <d3d12.h>
#include <wrl/client.h>
#include <d3dx12.h>

using Microsoft::WRL::ComPtr;

namespace TwoDX
{
	class D3D12PipelineState
	{
	public:
		D3D12PipelineState(
			D3D12Device* pD3D12Device,
			D3D12RootSignature* pRootSignature,
			ComPtr<ID3DBlob>& pVS,
			ComPtr<ID3DBlob>& pPS,
			UINT numRTV,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType,
			CD3DX12_RASTERIZER_DESC& rasterizer,
			CD3DX12_BLEND_DESC& blend,
			std::vector<D3D12_INPUT_ELEMENT_DESC>& elements,
			bool depthEnable = false,
			bool stencilEnable = false
		);
		~D3D12PipelineState();

		ID3D12PipelineState* GetPSO() const;

	private:
		struct Impl;

		std::unique_ptr<Impl> pImpl = nullptr;
	};
}
