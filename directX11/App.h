//========================================================================
// App.h
// メイン処理
//
// 更新日：2020/08/02
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
	std::vector < std::unique_ptr<class CBox>> m_boxes;
};