//========================================================================
// Box.h
// 
//
// 更新日：2020/08/12
// 栗城 達也
//========================================================================
#pragma once
#include "DrawableBase.h"

class CBox : public CDrawableBase<CBox>
{
public:
	CBox(CGraphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_real_distribution<float>& bdist,
		DirectX::XMFLOAT3 material);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	// 位置
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// スピード(delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;		
	// モデルトランスフォーム
	DirectX::XMFLOAT3X3 m_mt;
};