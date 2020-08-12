//========================================================================
// Cylinder.h
// 
//
// 更新日：2020/08/12
// 栗城 達也
//========================================================================
#pragma once
#include "TestObject.h"

class CCylinder : public CTestObject<CCylinder>
{
public:
	CCylinder(CGraphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_real_distribution<float>& bdist,
		std::uniform_int_distribution<int>& tdist);
};