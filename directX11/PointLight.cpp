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
{}

void CPointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &m_pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &m_pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &m_pos.z, -60.0f, 60.0f, "%.1f");
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void CPointLight::Reset() noexcept
{
	m_pos = { 0.0f, 0.0f, 0.0f };
}

void CPointLight::Draw(CGraphics& gfx) const noexcept(!IS_DEBUG)
{
	m_mesh.SetPos(m_pos);
	m_mesh.Draw(gfx);
}

void CPointLight::Bind(CGraphics& gfx) const noexcept
{
	m_cbuf.Update(gfx, PointLightCBuf{ m_pos });
	m_cbuf.Bind(gfx);
}