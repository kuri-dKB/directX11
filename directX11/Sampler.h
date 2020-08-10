//========================================================================
// Sampler.h
// 
//
// 更新日：2020/08/10
// 栗城 達也
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