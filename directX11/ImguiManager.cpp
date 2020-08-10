//========================================================================
// ImguiManager.cpp
//
// �X�V���F2020/08/10
// �I�� �B��
//========================================================================
#include "ImguiManager.h"
#include "imgui/imgui.h"

CImguiManager::CImguiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

CImguiManager::~CImguiManager()
{
	ImGui::DestroyContext();
}