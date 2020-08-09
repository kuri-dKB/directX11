//========================================================================
// Cone.h
// 
//
// �X�V���F2020/08/09
// �I�� �B��
//========================================================================
#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include "ChiliMath.h"

class CCone
{
public:
	template<class V>
	static CIndexedTriangleList<V> MakeTesselated(int longDiv)
	{
		namespace dx = DirectX;
		assert(longDiv >= 3);

		const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const float longitudeAngle = 2.0f * PI / longDiv;

		// �x�[�X���_
		std::vector<V> vertices;
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			dx::XMStoreFloat3(&vertices.back().pos, v);
		}
		// �Z���^�[
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, -1.0f };
		const auto iCenter = (unsigned short)(vertices.size() - 1);
		// ��[
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, 1.0f };
		const auto iTip = (unsigned short)(vertices.size() - 1);


		// �x�[�X�C���f�b�N�X
		std::vector<unsigned short> indices;
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			indices.push_back(iCenter);
			indices.push_back(iLong + 1) % longDiv;
			indices.push_back(iLong);
		}

		// cone�C���f�b�N�X
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			indices.push_back(iLong);
			indices.push_back(iLong + 1) % longDiv;
			indices.push_back(iTip);
		}

		return { std::move(vertices), std::move(indices) };
	}
	template<class V>
	static CIndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(24);
	}
};