//========================================================================
// SolidSphere.cpp
// 
//
// 更新日：2020/08/12
// 栗城 達也
//========================================================================
#include "SolidSphere.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "Sphere.h"

CSolidSphere::CSolidSphere(CGraphics& gfx, float radius)
{
	namespace dx = DirectX;

	if (!IsStaticInit())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
		};
		auto model = CSphere::Make<Vertex>();
		model.Transform(dx::XMMatrixScaling(radius, radius, radius));
		AddBind(std::make_unique<CVertexBuffer>(gfx, model.m_vertices));
		AddIndexBuffer(std::make_unique<CIndexBuffer>(gfx, model.m_indices));

		auto pvs = std::make_unique<CVertexShader>(gfx, L"SolidVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<CPixelShader>(gfx, L"SolidPS.cso"));

		struct PSColorConstant
		{
			dx::XMFLOAT3 color = { 1.0f, 1.0f, 1.0f };
			float padding;
		}colorConst;
		AddStaticBind(std::make_unique<CPixelConstantBuffer<PSColorConstant>>(gfx, colorConst));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		AddStaticBind(std::make_unique<CInputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<CTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<CTransformCbuf>(gfx, *this));
}

void CSolidSphere::Update(float dt) noexcept{}

void CSolidSphere::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}

DirectX::XMMATRIX CSolidSphere::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}