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
	CWindow m_wnd;
	CTimer  m_timer;
	std::vector < std::unique_ptr<class CDrawable>> m_drawables;
	static constexpr size_t nDrawables = 180; // モデルの個数
};