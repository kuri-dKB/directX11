//========================================================================
// App.h
// ���C������
//
// �X�V���F2020/08/09
// �I�� �B��
//========================================================================
#pragma once
#include "window.h"
#include "ChiliTimer.h"
#include "ImguiManager.h"

class CApp
{
public:
	CApp();
	// ���b�Z�[�W���[�v
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
	static constexpr size_t nDrawables = 180; // ���f���̌�
};