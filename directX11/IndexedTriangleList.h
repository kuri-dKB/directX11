//========================================================================
// IndexedTriangleList.h
// 
//
// 更新日：2020/08/08
// 栗城 達也
//========================================================================
#pragma once
#include <vector>
#include <DirectXMath.h>

template<class T>
class CIndexedTriangleList
{
public:
	CIndexedTriangleList() = default;
	CIndexedTriangleList(std::vector<T> verts_in, std::vector<unsigned short> indices_in)
		:
		m_vertices(std::move(verts_in)),
		m_indices(std::move(indices_in))
	{
		assert(m_vertices.size() > 2);
		assert(m_indices.size() % 3 == 0);
	}
	void Transform(DirectX::FXMMATRIX matrix)
	{
		for (auto& v : m_vertices)
		{
			const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&v.pos);
			DirectX::XMStoreFloat3(
				&v.pos,
				DirectX::XMVector3Transform(pos, matrix)
			);
		}
	}
public:
	std::vector<T> m_vertices;
	std::vector<unsigned short> m_indices;
};