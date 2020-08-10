//========================================================================
// Sampler.cpp
// 
//
// 更新日：2020/08/10
// 栗城 達也
//========================================================================
#include "Sampler.h"
#include "GraphicsThrowMacros.h"

CSampler::CSampler(CGraphics& gfx)
{
	INFOMAN(gfx);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &m_pSampler));
}

void CSampler::Bind(CGraphics& gfx) noexcept
{
	GetContext(gfx)->PSSetSamplers(0, 1, m_pSampler.GetAddressOf());
}