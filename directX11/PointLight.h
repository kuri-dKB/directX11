//========================================================================
// PointLight.h
// 
//
// 更新日：2020/08/12
// 栗城 達也
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
	void Bind(CGraphics& gfx) const noexcept;
private:
	struct PointLightCBuf
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 material;
		DirectX::XMFLOAT3 ambient;
		DirectX::XMFLOAT3 diffuseColor;
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