//========================================================================
// Drawable.h
// 
//
// 更新日：2020/08/14
// 栗城 達也
//========================================================================
#pragma once
#include "Graphics.h"
#include <DirectXMath.h>

class CBindable;

class CDrawable
{
	template<class T>
	friend class CDrawableBase;
public:
	CDrawable() = default;
	CDrawable(const CDrawable&) = delete;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void Draw(CGraphics& gfx) const noexcept(!IS_DEBUG);
	virtual void Update(float dt) noexcept = 0;
	virtual ~CDrawable() = default;
protected:
	template<class T>
	T* QueryBindable() noexcept
	{
		for (auto& pb : m_binds)
		{
			if (auto pt = dynamic_cast<T*>(pb.get()))
			{
				return pt;
			}
		}
		return nullptr;
	}
	void AddBind(std::unique_ptr<CBindable> bind) noexcept(!IS_DEBUG);
	void AddIndexBuffer(std::unique_ptr<class CIndexBuffer> ibuf) noexcept(!IS_DEBUG);
private:
	virtual const std::vector<std::unique_ptr<CBindable>>& GetStaticBinds() const noexcept = 0;
private:
	const class CIndexBuffer* m_pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<CBindable>> m_binds;
};