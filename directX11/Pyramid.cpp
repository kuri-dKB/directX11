//========================================================================
// Pyramid.h
// 
//
// 更新日：2020/08/12
// 栗城 達也
//========================================================================
#include "Pyramid.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "Cone.h"
#include <array>


CPyramid::CPyramid(CGraphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_int_distribution<int>& tdist)
	:
	CTestObject(gfx, rng, adist, ddist, odist, rdist)
{
	namespace dx = DirectX;

	if (!IsStaticInit())
	{
		auto pvs = std::make_unique<CVertexShader>(gfx, L"BlendedPhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<CPixelShader>(gfx, L"BlendedPhongPS.cso"));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<CInputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<CTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct PSMaterialConstant
		{
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[2];
		}colorConst;
		AddStaticBind(std::make_unique<CPixelConstantBuffer<PSMaterialConstant>>(gfx, colorConst, 1u));
	}
	
	struct Vertex
	{
		dx::XMFLOAT3 pos;
		dx::XMFLOAT3 n;
		std::array<char, 4> color;
		char padding;
	};
	const auto tesselation = tdist(rng);
	auto model = CCone::MakeTesselatedIndependentFaces<Vertex>(tesselation);
	// 色
	for (auto& v : model.m_vertices)
	{
		v.color = { (char)10, (char)10, (char)255 };
	}
	for (int i = 0; i < tesselation; i++)
	{
		model.m_vertices[i * 3].color = { (char)255, (char)10, (char)10 }; // 先端
	}
	// Z軸方向に少し伸ばす
	model.Transform(dx::XMMatrixScaling(1.0f, 1.0f, 0.7f));
	// ノーマル
	model.SetNormalsIndependentFlat();

	AddBind(std::make_unique<CVertexBuffer>(gfx, model.m_vertices));
	AddIndexBuffer(std::make_unique<CIndexBuffer>(gfx, model.m_indices));

	AddBind(std::make_unique<CTransformCbuf>(gfx, *this));
}