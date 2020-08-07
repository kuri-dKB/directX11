//========================================================================
// PixelShader.cpp
// 
//
// 更新日：2020/08/08
// 栗城 達也
//========================================================================
#include "PixelShader.h"
#include "GraphicsThrowMacros.h"

CPixelShader::CPixelShader(CGraphics& gfx, const std::wstring & path)
{
	INFOMAN(gfx);

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBlob));
	GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader));
}

void CPixelShader::Bind(CGraphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
}
