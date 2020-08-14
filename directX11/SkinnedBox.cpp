//========================================================================
// SkinnedBox.cpp
// 
//
// 更新日：2020/08/14
// 栗城 達也
//========================================================================
#include "SkinnedBox.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "Cube.h"
#include "Surface.h"
#include "Texture.h"
#include "Sampler.h"


CSkinnedBox::CSkinnedBox(CGraphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist)
	:
	CTestObject(gfx, rng, adist, ddist, odist, rdist)
{
	namespace dx = DirectX;

	if (!IsStaticInit())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 n;
			dx::XMFLOAT2 tc;
		};
		auto model = CCube::MakeIndependentTextured<Vertex>();
		model.SetNormalsIndependentFlat();

		AddStaticBind(std::make_unique<CVertexBuffer>(gfx, model.m_vertices));

		AddStaticBind(std::make_unique<CTexture>(gfx, CSurface::FromFile("Images\\kappa50.png")));

		AddStaticBind(std::make_unique<CSampler>(gfx));

		auto pvs = std::make_unique<CVertexShader>(gfx, L"TexturedPhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<CPixelShader>(gfx, L"TexturedPhongPS.cso"));

		AddStaticIndexBuffer(std::make_unique<CIndexBuffer>(gfx, model.m_indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<CInputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<CTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct PSMaterialConstant
		{
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[2];
		} colorConst;
		AddStaticBind(std::make_unique<CPixelConstantBuffer<PSMaterialConstant>>(gfx, colorConst, 1u));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<CTransformCbuf>(gfx, *this));
}
