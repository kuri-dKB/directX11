//========================================================================
// Bindable.h
// 
//
// 更新日：2020/08/05
// 栗城 達也
//========================================================================
#pragma once
#include "Graphics.h"

class CBindable
{
public:
	virtual void Bind(CGraphics& gfx) noexcept = 0;
	virtual ~CBindable() = default;
protected:
	static ID3D11DeviceContext* GetContext(CGraphics& gfx) noexcept;
	static ID3D11Device*        GetDevice(CGraphics& gfx) noexcept;
	static CDxgiInfoManager&    GetInfoManager(CGraphics& gfx) noexcept(!IS_DEBUG);
};