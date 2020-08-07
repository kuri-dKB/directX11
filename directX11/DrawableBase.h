//========================================================================
// DrawableBase.h
// 
//
// 更新日：2020/08/07
// 栗城 達也
//========================================================================
#pragma once
#include "Drawable.h"
#include "IndexBuffer.h"

template<class T>
class CDrawableBase : public CDrawable
{
protected:
	static bool IsStaticInit() noexcept
	{
		return !m_staticBinds.empty();
	}
	static void AddStaticBind(std::unique_ptr<CBindable> bind) noexcept(!IS_DEBUG)
	{
		assert("*Must* インデックスバッファをバインドするためにAddStaticIndexBufferを使え！" && typeid(*bind) != typeid(CIndexBuffer));
		m_staticBinds.push_back(std::move(bind));
	}
	void AddStaticIndexBuffer(std::unique_ptr<CIndexBuffer> ibuf) noexcept(!IS_DEBUG)
	{
		assert("2回目のインデックスバッファの追加をしてみる" && m_pIndexBuffer == nullptr);
		m_pIndexBuffer = ibuf.get();
		m_staticBinds.push_back(std::move(ibuf));
	}
	void SetIndexFromStatic() noexcept(!IS_DEBUG)
	{
		assert("2回目のインデックスバッファの追加をしてみる" && m_pIndexBuffer == nullptr);
		for (const auto& b : m_staticBinds)
		{
			if (const auto p = dynamic_cast<CIndexBuffer*>(b.get()))
			{
				m_pIndexBuffer = p;
				return;
			}
		}
		assert("インデックスバッファが見つからなかった" && m_pIndexBuffer != nullptr);
	}
private:
	const std::vector<std::unique_ptr<CBindable>>& GetStaticBinds() const noexcept override
	{
		return m_staticBinds;
	}
private:
	static std::vector<std::unique_ptr<CBindable>> m_staticBinds;
};

template<class T>
std::vector<std::unique_ptr<CBindable>> CDrawableBase<T>::m_staticBinds;