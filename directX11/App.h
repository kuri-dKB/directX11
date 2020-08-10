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
	bool show_demo_window = true;
	static constexpr size_t nDrawables = 180; // モデルの個数
};