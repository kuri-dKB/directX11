//========================================================================
// VertexShader.h
// 
//
// �X�V���F2020/08/05
// �I�� �B��
//========================================================================
#include "VertexShader.h"
#include "GraphicsThrowMacros.h"

CVertexShader::CVertexShader(CGraphics & gfx, const std::wstring & path)
{
	INFOMAN(gfx);

	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &m_pBytecodeBlob));
	GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(
		m_pBytecodeBlob->GetBufferPointer(),
		m_pBytecodeBlob->GetBufferSize(),
		nullptr,
		&m_pVertexShader
	));
}

void CVertexShader::Bind(CGraphics & gfx) noexcept
{
	GetContext(gfx)->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob * CVertexShader::GetBytecode() const noexcept
{
	return m_pBytecodeBlob.Get();
}
