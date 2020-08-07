//========================================================================
// TransformCbuf.h
// 
//
// 更新日：2020/08/05
// 栗城 達也
//========================================================================
#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

class CTransformCbuf : public CBindable
{
public:
	CTransformCbuf(CGraphics& gfx, const CDrawable& parent);
	void Bind(CGraphics& gfx) noexcept override;
private:
	static std::unique_ptr<CVertexConstantBuffer<DirectX::XMMATRIX>> m_Vcbuf;
	const CDrawable& m_parent;
};