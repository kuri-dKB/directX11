#pragma once
#include "window.h"
#include "ChiliTimer.h"

class CApp
{
public:
	CApp();
	// master frame / message loop
	int Go();
private:
	void DoFrame();
private:
	CWindow m_wnd;
	CTimer  m_timer;
};