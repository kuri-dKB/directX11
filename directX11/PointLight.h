//========================================================================
// PointLight.h
// 
//
// �X�V���F2020/08/12
// �I�� �B��
//========================================================================
#pragma once
#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"

class CPointLight
{
public:
	CPointLight(CGraphics& gfx, float radius = 0.5f);
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Draw(CGraphics& gfx) const noexcept(!IS_DEBUG);
	void Bind(CGraphics& gfx, DirectX::FXMMATRIX view) const noexcept;
private:
	struct PointLightCBuf
	{
		alignas(16) DirectX::XMFLOAT3 pos; // �V�F�[�_���ƃo�C�g�������킹��
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};
private:
	PointLightCBuf m_cbData;
	mutable CSolidSphere m_mesh;
	mutable CPixelConstantBuffer<PointLightCBuf> m_cbuf;
};