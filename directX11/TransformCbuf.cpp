//========================================================================
// TransformCbuf.cpp
// 
//
// 更新日：2020/08/12
// 栗城 達也
//========================================================================
#include "TransformCbuf.h"

CTransformCbuf::CTransformCbuf(CGraphics & gfx, const CDrawable & parent, UINT slot)
	:
	m_parent(parent)
{
	if (!m_Vcbuf)
	{
		m_Vcbuf = std::make_unique<CVertexConstantBuffer<Transforms>>(gfx, slot);
	}
}

void CTransformCbuf::Bind(CGraphics & gfx) noexcept
{
	const auto modelView = m_parent.GetTransformXM() * gfx.GetCamera();
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(
			modelView *
			gfx.GetProjection()
		)
	};
	m_Vcbuf->Update(gfx, tf);
	m_Vcbuf->Bind(gfx);
}

std::unique_ptr<CVertexConstantBuffer<CTransformCbuf::Transforms>> CTransformCbuf::m_Vcbuf;