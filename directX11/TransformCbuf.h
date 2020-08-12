//========================================================================
// TransformCbuf.h
// 
//
// 更新日：2020/08/12
// 栗城 達也
//========================================================================
#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

class CTransformCbuf : public CBindable
{
public:
	struct Transforms
	{
		DirectX::XMMATRIX modelViewProj;
		DirectX::XMMATRIX model;
	};
	CTransformCbuf(CGraphics& gfx, const CDrawable& parent, UINT slot = 0u);
	void Bind(CGraphics& gfx) noexcept override;
private:
	static std::unique_ptr<CVertexConstantBuffer<Transforms>> m_Vcbuf;
	const CDrawable& m_parent;
};