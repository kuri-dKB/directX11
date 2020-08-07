//========================================================================
// Drawable.cpp
// 
//
// �X�V���F2020/08/05
// �I�� �B��
//========================================================================
#include "Drawable.h"
#include "GraphicsThrowMacros.h"
#include "IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void CDrawable::Draw(CGraphics & gfx) const noexcept(!IS_DEBUG)
{
	for (auto & b : m_binds)
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

void CDrawable::AddIndexBuffer(std::unique_ptr<class CIndexBuffer> ibuf) noexcept
{
	assert("2��ڂ̃C���f�b�N�X�o�b�t�@�̒ǉ������܂�" && m_pIndexBuffer == nullptr);
	m_pIndexBuffer = ibuf.get();
	m_binds.push_back(std::move(ibuf));
}
