#pragma once
#include <Device/D3D12Device.h>
#include <memory>
#include <d3d12.h>
#include <d3dx12.h>

namespace TwoDX
{
	class D3D12RootSignature
	{
	public:
		D3D12RootSignature(
			D3D12Device* pD3D12Device,
			UINT numParams = 0,
			CD3DX12_ROOT_PARAMETER* params = nullptr,
			UINT numSamplers = 0,
			CD3DX12_STATIC_SAMPLER_DESC* samplers = nullptr
		);
		~D3D12RootSignature();

		ID3D12RootSignature* GetRootSignature() const;

	private:
		struct Impl;

		std::unique_ptr<Impl> pImpl = nullptr;
	};
}
