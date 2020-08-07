//========================================================================
// PixelShader.h
// 
//
// �X�V���F2020/08/08
// �I�� �B��
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
