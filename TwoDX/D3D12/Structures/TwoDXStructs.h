#pragma once
#include <d3d12.h>

namespace TwoDX
{
	struct D3D12Viewport : public D3D12_VIEWPORT
	{
		D3D12Viewport(
			FLOAT width,
			FLOAT height,
			FLOAT topLeftX,
			FLOAT topLeftY,
			FLOAT minDepth,
			FLOAT maxDepth
		)
		{
			Width = width;
			Height = height;
			TopLeftX = topLeftX;
			TopLeftY = topLeftY;
			MinDepth = minDepth;
			MaxDepth = maxDepth;
		}
	};

	struct D3D12Rect : public D3D12_RECT
	{
		D3D12Rect(
			LONG left,
			LONG right,
			LONG top,
			LONG bottom
		)
		{
			this->left = left;
			this->right = right;
			this->top = top;
			this->bottom = bottom;
		}
	};

	struct Vertex3D
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 color;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};
}