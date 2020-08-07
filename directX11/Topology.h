//========================================================================
// Topology.h
// 
//
// 更新日：2020/08/05
// 栗城 達也
//========================================================================
#pragma once
#include "Bindable.h"

class CTopology : public CBindable
{
public:
	CTopology(CGraphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
	void Bind(CGraphics& gfx) noexcept override;
protected:
	D3D11_PRIMITIVE_TOPOLOGY m_type;
};