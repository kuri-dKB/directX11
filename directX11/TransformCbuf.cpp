//========================================================================
// TransformCbuf.cpp
// 
//
// 更新日：2020/08/05
// 栗城 達也
//========================================================================
#include "TransformCbuf.h"

CTransformCbuf::CTransformCbuf(CGraphics & gfx, const CDrawable & parent)
	:
	m_parent(parent)
{
	if (!m_Vcbuf)
	{
		m_Vcbuf = std::make_unique<CVertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

void CTransformCbuf::Bind(CGraphics & gfx) noexcept
{
	m_Vcbuf->Update(gfx,
		DirectX::XMMatrixTranspose(
			m_parent.GetTransformXM() *
			gfx.GetCamera() *
			gfx.GetProjection()
		));
	m_Vcbuf->Bind(gfx);
}

std::unique_ptr<CVertexConstantBuffer<DirectX::XMMATRIX>> CTransformCbuf::m_Vcbuf;