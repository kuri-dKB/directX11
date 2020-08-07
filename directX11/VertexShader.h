//========================================================================
// VertexShader.h
// 
//
// 更新日：2020/08/05
// 栗城 達也
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