//========================================================================
// Topology.cpp
// 
//
// �X�V���F2020/08/05
// �I�� �B��
//========================================================================
#include "Topology.h"

CTopology::CTopology(CGraphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
	:
	m_type(type)
{}

void CTopology::Bind(CGraphics& gfx) noexcept
{
	GetContext(gfx)->IASetPrimitiveTopology(m_type);
}