#pragma once
#include <DXGInfrastructure/DXGInfrastructure.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <memory>

namespace TwoDX
{
	class D3D12Device
	{
	public:
		D3D12Device(DXGInfrastructure* pDXGI);
		~D3D12Device();

		ID3D12Device6* GetDevice() const;

	private:
		struct Impl;

		std::unique_ptr<Impl> pImpl = nullptr;
	};
}
