#pragma once
#include <memory>
#include <dxgi1_6.h>

namespace TwoDX
{
	class DXGInfrastructure
	{
	public:
		DXGInfrastructure();
		~DXGInfrastructure();

		IDXGIFactory6* GetFactory() const;
		IDXGIAdapter4* GetAdapter() const;

	private:
		struct Impl;

		std::unique_ptr<Impl> pImpl = nullptr;
	};
}
