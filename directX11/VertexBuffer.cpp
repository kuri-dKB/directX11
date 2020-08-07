//========================================================================
// VertexBuffer.cpp
// 
//
// �X�V���F2020/08/05
// �I�� �B��
//========================================================================
#include "VertexBuffer.h"

void CVertexBuffer::Bind(CGraphics& gfx) noexcept
{
	const UINT offset = 0u;
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, m_pVertexBuffer.GetAddressOf(), &m_stride, &offset);
}