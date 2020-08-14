//========================================================================
// Box.cpp
// 
//
// 更新日：2020/08/14
// 栗城 達也
//========================================================================
#include "Box.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "Cube.h"
#include "imgui/imgui.h"


CBox::CBox(CGraphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist,
	DirectX::XMFLOAT3 material)
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
		auto model = CCube::MakeIndependent<Vertex>();
		model.SetNormalsIndependentFlat();

		AddStaticBind(std::make_unique<CVertexBuffer>(gfx, model.m_vertices));

		auto pvs = std::make_unique<CVertexShader>(gfx, L"PhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<CPixelShader>(gfx, L"PhongPS.cso"));

		AddStaticIndexBuffer(std::make_unique<CIndexBuffer>(gfx, model.m_indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		AddStaticBind(std::make_unique<CInputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<CTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<CTransformCbuf>(gfx, *this));

	materialConstants.color = material;
	AddBind(std::make_unique<MaterialCBuf>(gfx, materialConstants, 1u));

	// モデル変形
	dx::XMStoreFloat3x3(
		&m_mt,
		dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng))
	);
}

DirectX::XMMATRIX CBox::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMLoadFloat3x3(&m_mt) * CTestObject::GetTransformXM();
}

bool CBox::SpawnControlerWindow(int id, CGraphics& gfx) noexcept
{
	using namespace std::string_literals;

	bool dirty = false;
	bool open = true;
	if (ImGui::Begin(("Box"s + std::to_string(id)).c_str(), &open));
	{
		const auto cd = ImGui::ColorEdit3("Material Color", &materialConstants.color.x);
		const auto sid = ImGui::SliderFloat("Specular Intensity", &materialConstants.specularIntensity, 0.05, 4.0f, "%.2f", 2);
		const auto spd = ImGui::SliderFloat("Specular Power", &materialConstants.specularPower, 1.0f, 200.0f, "%.2f", 2);
		dirty = cd || sid || spd;
	}
	ImGui::End();

	if (dirty)
	{
		SyncMaterial(gfx);
	}
	return open;
}

void CBox::SyncMaterial(CGraphics& gfx) noexcept(!IS_DEBUG)
{
	auto pConstPS = QueryBindable<MaterialCBuf>();
	assert(pConstPS != nullptr);
	pConstPS->Update(gfx, materialConstants);
}