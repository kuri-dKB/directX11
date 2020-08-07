//========================================================================
// TransformCbuf.h
// 
//
// �X�V���F2020/08/05
// �I�� �B��
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