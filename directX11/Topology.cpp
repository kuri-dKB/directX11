//========================================================================
// Topology.cpp
// 
//
// 更新日：2020/08/05
// 栗城 達也
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