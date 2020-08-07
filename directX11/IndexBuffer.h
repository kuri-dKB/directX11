//========================================================================
// IndexBuffer.h
// 
//
// �X�V���F2020/08/05
// �I�� �B��
//========================================================================
#pragma once
#include "Bindable.h"

class CIndexBuffer : public CBindable
{
public:
	CIndexBuffer(CGraphics& gfx, const std::vector<unsigned short>& indices);
	void Bind(CGraphics& gfx) noexcept override;
	UINT GetCount() const noexcept;
protected:
	UINT m_count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
};