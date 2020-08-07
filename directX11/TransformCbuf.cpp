//========================================================================
// TransformCbuf.cpp
// 
//
// �X�V���F2020/08/05
// �I�� �B��
//========================================================================
#include "TransformCbuf.h"

CTransformCbuf::CTransformCbuf(CGraphics & gfx, const CDrawable & parent)
	:
	m_vcbuf(gfx),
	m_parent(parent)
{}

void CTransformCbuf::Bind(CGraphics & gfx) noexcept
{
	m_vcbuf.Update(gfx,
		DirectX::XMMatrixTranspose(
			m_parent.GetTransformXM() * gfx.GetProjection()
		));
	m_vcbuf.Bind(gfx);
}
