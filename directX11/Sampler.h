//========================================================================
// Sampler.h
// 
//
// �X�V���F2020/08/10
// �I�� �B��
//========================================================================
#pragma once
#include "Bindable.h"

class CSampler : public CBindable
{
public:
	CSampler(CGraphics& gfx);
	void Bind(CGraphics& gfx) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler;
};