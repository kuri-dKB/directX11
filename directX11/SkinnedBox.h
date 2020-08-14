//========================================================================
// SkinnedBox.h
// 
//
// �X�V���F2020/08/14
// �I�� �B��
//========================================================================
#pragma once
#include "TestObject.h"

class CSkinnedBox : public CTestObject<CSkinnedBox>
{
public:
	CSkinnedBox(CGraphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist);
};