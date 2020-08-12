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
		// 強さ 色
		ImGui::Text("Intensity/Color");
		ImGui::SliderFloat("Intensity", &m_cbData.diffuseIntensity, 0.01f, 2.0f, "%.2f", 2);
		ImGui::ColorEdit3("Diffuse Color", &m_cbData.diffuseColor.x);
		ImGui::ColorEdit3("Ambient", &m_cbData.ambient.x);
		// 減衰
		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &m_cbData.attConst, 0.05f, 10.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear", &m_cbData.attLin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &m_cbData.attQuad, 0.0000001, 10.0f, "%.7f", 10);
	
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