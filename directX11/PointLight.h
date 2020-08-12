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
		float padding;
	};
private:
	DirectX::XMFLOAT3 m_pos = { 0.0f, 0.0f, 0.0f };
	mutable CSolidSphere m_mesh;
	mutable CPixelConstantBuffer<PointLightCBuf> m_cbuf;
};