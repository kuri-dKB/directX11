//========================================================================
// Texture.h
// 
//
// �X�V���F2020/08/10
// �I�� �B��
//========================================================================
#pragma once
#include "Bindable.h"

class CTexture : public CBindable
{
public:
	CTexture(CGraphics& gfx, const class CSurface& s);
	void Bind(CGraphics& gfx) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView;
};
