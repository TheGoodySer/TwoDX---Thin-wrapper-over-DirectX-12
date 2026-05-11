#pragma once
#include <Device/D3D12Device.h>
#include <CommandQueue/D3D12CommandQueue.h>
#include <memory>
#include <string>

namespace TwoDX
{
	class Texture2D
	{
	public:
		Texture2D(
			D3D12Device* pD3D12Device,
			D3D12CommandQueue* pCmdQueue,
			const std::wstring& texPath
		);
		~Texture2D();

		ID3D12Resource* GetResource() const;

		const D3D12_RESOURCE_DESC& GetDesc() const;

	private:
		struct Impl;

		std::unique_ptr<Impl> pImpl = nullptr;
	};
}
