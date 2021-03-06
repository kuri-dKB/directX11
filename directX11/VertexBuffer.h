//========================================================================
// VertexBuffer.h
// 
//
// �X�V���F2020/08/05
// �I�� �B��
//========================================================================
#pragma once
#include "Bindable.h"
#include "GraphicsThrowMacros.h"

class CVertexBuffer : public CBindable
{
public:
	template<class V>
	CVertexBuffer(CGraphics& gfx, const std::vector<V>& vertices)
		:
		m_stride(sizeof(V))
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(V) * vertices.size());
		bd.StructureByteStride = sizeof(V);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &sd, &m_pVertexBuffer));
	}
	void Bind(CGraphics& gfx) noexcept override;
protected:
	UINT m_stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
};