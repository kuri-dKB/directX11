//========================================================================
// PointLight.cpp
// 
//
// 更新日：2020/08/12
// 栗城 達也
//========================================================================
#include "Pointlight.h"
#include "imgui/imgui.h"

CPointLight::CPointLight(CGraphics& gfx, float radius)
	:
	m_mesh(gfx, radius),
	m_cbuf(gfx)
{
	Reset();
}

void CPointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &m_cbData.pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &m_cbData.pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &m_cbData.pos.z, -60.0f, 60.0f, "%.1f");
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void CPointLight::Reset() noexcept
{
	m_cbData = {
		{ 0.0f, 0.0f, 0.0f },
		{ 0.7f, 0.7f, 0.9f },
		{ 0.05f, 0.05f, 0.05f },
		{ 1.0f, 1.0f, 1.0f },
		1.0f,
		1.0f,
		0.045f,
		0.0075f,
	};
}

void CPointLight::Draw(CGraphics& gfx) const noexcept(!IS_DEBUG)
{
	m_mesh.SetPos(m_cbData.pos);
	m_mesh.Draw(gfx);
}

void CPointLight::Bind(CGraphics& gfx) const noexcept
{
	m_cbuf.Update(gfx, PointLightCBuf{ m_cbData });
	m_cbuf.Bind(gfx);
}