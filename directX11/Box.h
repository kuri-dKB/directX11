//========================================================================
// Box.h
// 
//
// �X�V���F2020/08/14
// �I�� �B��
//========================================================================
#pragma once
#include "TestObject.h"
#include "ConstantBuffers.h"

class CBox : public CTestObject<CBox>
{
public:
	CBox(CGraphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_real_distribution<float>& bdist,
		DirectX::XMFLOAT3 material);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	// �E�B���h�E������ꂽ�� false ��Ԃ�
	bool SpawnControlerWindow(int id, CGraphics& gfx) noexcept;
private:
	void SyncMaterial(CGraphics& gfx) noexcept(!IS_DEBUG);
private:
	struct PSMaterialConstant
	{
		DirectX::XMFLOAT3 color;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[3];
	} materialConstants;
	using MaterialCBuf = CPixelConstantBuffer<PSMaterialConstant>;
private:
	// ���f���g�����X�t�H�[��
	DirectX::XMFLOAT3X3 m_mt;
};