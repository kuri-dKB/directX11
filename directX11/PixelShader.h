//========================================================================
// PixelShader.h
// 
//
// 更新日：2020/08/08
// 栗城 達也
//========================================================================
#pragma once
#include "Bindable.h"

class CPixelShader : public CBindable
{
public:
	CPixelShader(CGraphics& gfx, const std::wstring& path);
	void Bind(CGraphics& gfx) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
};
