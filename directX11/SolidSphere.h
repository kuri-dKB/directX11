//========================================================================
// SolidSphere.h
// 
//
// 更新日：2020/08/12
// 栗城 達也
//========================================================================
#pragma once
#include "DrawableBase.h"

class CSolidSphere : public CDrawableBase<CSolidSphere>
{
public:
	CSolidSphere(CGraphics& gfx, float radius);
	void Update(float dt) noexcept override;
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
};