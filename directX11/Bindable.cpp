//========================================================================
// Bindable.cpp
// 
//
// 更新日：2020/08/05
// 栗城 達也
//========================================================================
#include "Bindable.h"

ID3D11DeviceContext * CBindable::GetContext(CGraphics & gfx) noexcept
{
	return gfx.m_pContext.Get();
}

ID3D11Device * CBindable::GetDevice(CGraphics & gfx) noexcept
{
	return gfx.m_pDevice.Get();
}

CDxgiInfoManager & CBindable::GetInfoManager(CGraphics & gfx) noexcept(!IS_DEBUG)
{
#ifndef NDEBUG
	return gfx.m_infoManager;
#else
	throw std::logic_error("滅茶苦茶だ！ (Release configのgfx.m_infoManagerを見てみて)");
#endif // !NDEBUG
}
