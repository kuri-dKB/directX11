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
		m_Vcbuf = std::make_unique<CVertexConstantBuffer<Transforms>>(gfx);
	}
}

void CTransformCbuf::Bind(CGraphics & gfx) noexcept
{
	const auto model = m_parent.GetTransformXM();
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(model),
		DirectX::XMMatrixTranspose(
			model *
			gfx.GetCamera() *
			gfx.GetProjection()
		)
	};
	m_Vcbuf->Update(gfx, tf);
	m_Vcbuf->Bind(gfx);
}

std::unique_ptr<CVertexConstantBuffer<CTransformCbuf::Transforms>> CTransformCbuf::m_Vcbuf;