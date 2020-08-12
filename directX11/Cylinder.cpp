//========================================================================
// Cylinder.cpp
// 
//
// 更新日：2020/08/12
// 栗城 達也
//========================================================================
#include "Cylinder.h"
#include "Prism.h"
#include "BindableBase.h"

CCylinder::CCylinder(CGraphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist,
	std::uniform_int_distribution<int>& tdist)
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
		};
		auto model = CPrism::MakeTesselatedIndependentCapNormals<Vertex>(tdist(rng));

		AddStaticBind(std::make_unique<CVertexBuffer>(gfx, model.m_vertices));

		auto pvs = std::make_unique<CVertexShader>(gfx, L"PhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<CPixelShader>(gfx, L"IndexedPhongPS.cso"));

		AddStaticIndexBuffer(std::make_unique<CIndexBuffer>(gfx, model.m_indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<CInputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<CTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct PSMaterialConstant
		{
			dx::XMFLOAT3A colors[6] = {
				{1.0f,0.0f,0.0f},
				{0.0f,1.0f,0.0f},
				{0.0f,0.0f,1.0f},
				{1.0f,1.0f,0.0f},
				{1.0f,0.0f,1.0f},
				{0.0f,1.0f,1.0f},
			};
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
		} matConst;
		AddStaticBind(std::make_unique<CPixelConstantBuffer<PSMaterialConstant>>(gfx, matConst, 1u));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<CTransformCbuf>(gfx, *this));
}
