//========================================================================
// InputLayout.cpp
// 
//
// �X�V���F2020/08/05
// �I�� �B��
//========================================================================
#include "InputLayout.h"
#include "GraphicsThrowMacros.h"

CInputLayout::CInputLayout(CGraphics& gfx, 
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, 
	ID3DBlob * pVertexShaderBytecode)
{
	INFOMAN(gfx);

	GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
		layout.data(), (UINT)layout.size(),
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&m_pInputLayout
	));
}

void CInputLayout::Bind(CGraphics & gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(m_pInputLayout.Get());
}
