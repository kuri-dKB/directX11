//========================================================================
// Drawable.h
// 
//
// 更新日：2020/08/05
// 栗城 達也
//========================================================================
#pragma once
#include "Graphics.h"
#include <DirectXMath.h>

class CBindable;

class CDrawable
{
public:
	CDrawable() = default;
	CDrawable(const CDrawable&) = delete;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void Draw(CGraphics& gfx) const noexcept(!IS_DEBUG);
	virtual void Update(float dt) noexcept = 0;
	void AddBind(std::unique_ptr<CBindable> bind) noexcept(!IS_DEBUG);
	void AddIndexBuffer(std::unique_ptr<class CIndexBuffer> ibuf) noexcept;
	virtual ~CDrawable() = default;
private:
	const CIndexBuffer* m_pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<CBindable>> m_binds;
};