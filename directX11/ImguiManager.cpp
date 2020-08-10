//========================================================================
// ImguiManager.cpp
//
// 更新日：2020/08/10
// 栗城 達也
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