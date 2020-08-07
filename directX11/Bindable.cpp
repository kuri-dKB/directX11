//========================================================================
// Bindable.cpp
// 
//
// �X�V���F2020/08/05
// �I�� �B��
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
	throw std::logic_error("�Œ��ꒃ���I (Release config��gfx.m_infoManager�����Ă݂�)");
#endif // !NDEBUG
}
