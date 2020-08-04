//========================================================================
// App.h
// メイン処理
//
// 更新日：2020/08/04
// 栗城 達也
//========================================================================
#include "App.h"
#include <sstream>
#include <iomanip>

CApp::CApp()
	:
	m_wnd(800,600, "The Donkey Fart Box")
{}

int CApp::Go()
{
	while(true)
	{
		// 保留中のすべてのメッセージを処理する
		if (const auto ecode = CWindow::ProcessMessages())
		{
			// return optionalに値があれば終了->returnする
			return *ecode;
		}
		DoFrame();
	}
}

void CApp::DoFrame()
{
	const float c = sin(m_timer.Peek()) / 2.0f + 0.5f;
	m_wnd.Gfx().ClearBuffer(c, c, 1.0f);
	m_wnd.Gfx().DrawTextTriangle
	(
		-m_timer.Peek(),
		0.0f,
		0.0f
	);
	m_wnd.Gfx().DrawTextTriangle
	(
		m_timer.Peek(),
		m_wnd.m_mouse.GetPosX() / 400.0f - 1.0f,
		-m_wnd.m_mouse.GetPosY() / 300.0f + 1.0f
	);
	m_wnd.Gfx().EndFrame();
}