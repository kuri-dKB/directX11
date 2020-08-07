//========================================================================
// VertexShader.h
// 
//
// �X�V���F2020/08/05
// �I�� �B��
//========================================================================
#pragma once
#include "Bindable.h"

class CVertexShader : public CBindable
{
public:
	CVertexShader(CGraphics& gfx, const std::wstring& path);
	void Bind(CGraphics& gfx) noexcept override;
	ID3DBlob* GetBytecode() const noexcept;
protected:
	Microsoft::WRL::ComPtr<ID3DBlob> m_pBytecodeBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
};