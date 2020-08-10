//========================================================================
// Sheet.h
// 
//
// �X�V���F2020/08/10
// �I�� �B��
//========================================================================
#pragma once
#include "DrawableBase.h"

class CSheet : public CDrawableBase<CSheet>
{
public:
	CSheet(CGraphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	// �ʒu
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// �X�s�[�h(delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};