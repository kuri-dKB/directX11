//========================================================================
// Drawable.cpp
// 
//
// 更新日：2020/08/05
// 栗城 達也
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
	assert("*Must* インデックスバッファをセットするためにAddIndexBufferを使って" && typeid(*bind) != typeid(CIndexBuffer));
	m_binds.push_back(std::move(bind));
}

void CDrawable::AddIndexBuffer(std::unique_ptr<class CIndexBuffer> ibuf) noexcept
{
	assert("2回目のインデックスバッファの追加をします" && m_pIndexBuffer == nullptr);
	m_pIndexBuffer = ibuf.get();
	m_binds.push_back(std::move(ibuf));
}
