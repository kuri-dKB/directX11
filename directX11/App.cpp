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
		// process all messages pending, but to not block
		if (const auto ecode = CWindow::ProcessMessages())
		{
			// if return optional has value, means we're
			return *ecode;
		}
		DoFrame();
	}
}

void CApp::DoFrame()
{
	const float c = sin(m_timer.Peek()) / 2.0f + 0.5f;
	m_wnd.Gfx().ClearBuffer(c, c, 1.0f);
	m_wnd.Gfx().DrawTextTriangle();
	m_wnd.Gfx().EndFrame();
}