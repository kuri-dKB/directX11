//========================================================================
// App.h
// メイン処理
//
// 更新日：2020/08/09
// 栗城 達也
//========================================================================
#pragma once
#include "window.h"
#include "ChiliTimer.h"
#include "ImguiManager.h"
#include "Camera.h"

class CApp
{
public:
	CApp();
	// メッセージループ
	int Go();
	~CApp();
private:
	void DoFrame();
private:
	CImguiManager m_imgui;
	CWindow m_wnd;
	CTimer  m_timer;
	std::vector < std::unique_ptr<class CDrawable>> m_drawables;
	float speed_factor = 1.0f;
	CCamera m_cam;
	static constexpr size_t nDrawables = 180; // モデルの個数
};