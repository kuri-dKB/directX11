//========================================================================
// InputLayout.h
// 
//
// �X�V���F2020/08/08
// �I�� �B��
//========================================================================
#pragma once
#include "Bindable.h"

class CInputLayout : public CBindable
{
public:
	CInputLayout(CGraphics& gfx,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
		ID3DBlob* pVertexShaderBytecode);
	void Bind(CGraphics& gfx) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
};