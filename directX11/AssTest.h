//========================================================================
// AssTest.h
// 
//
// 更新日：2020/08/19
// 栗城 達也
//========================================================================
#pragma once
#include "TestObject.h"
#include "ConstantBuffers.h"

class CAssTest : public CTestObject<CAssTest>
{
public:
	CAssTest(CGraphics& gfx, std::mt19937& rng,
			std::uniform_real_distribution<float>& adist,
			std::uniform_real_distribution<float>& ddist,
			std::uniform_real_distribution<float>& odist,
			std::uniform_real_distribution<float>& rdist,
			DirectX::XMFLOAT3 material,
			float scale);
};
