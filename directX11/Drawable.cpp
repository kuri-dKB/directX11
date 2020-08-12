//========================================================================
// Drawable.cpp
// 
//
// �X�V���F2020/08/12
// �I�� �B��
//========================================================================
#include "Drawable.h"
#include "GraphicsThrowMacros.h"
#include "IndexBuffer.h"
#include <cassert>

void CDrawable::Draw(CGraphics & gfx) const noexcept(!IS_DEBUG)
{
	for (auto & b : m_binds)
	{
		b->Bind(gfx);
	}
	for (auto& b : GetStaticBinds())
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexed(m_pIndexBuffer->GetCount());
}

void CDrawable::AddBind(std::unique_ptr<CBindable> bind) noexcept(!IS_DEBUG)
{
	assert("*Must* �C���f�b�N�X�o�b�t�@���Z�b�g���邽�߂�AddIndexBuffer���g����" && typeid(*bind) != typeid(CIndexBuffer));
	m_binds.push_back(std::move(bind));
}

void CDrawable::AddIndexBuffer(std::unique_ptr<CIndexBuffer> ibuf) noexcept(!IS_DEBUG)
{
	assert("2��ڂ̃C���f�b�N�X�o�b�t�@�̒ǉ������܂�" && m_pIndexBuffer == nullptr);
	m_pIndexBuffer = ibuf.get();
	m_binds.push_back(std::move(ibuf));
}
