//========================================================================
// App.h
// ���C������
//
// �X�V���F2020/08/02
// �I�� �B��
//========================================================================
#pragma once
#include "window.h"
#include "ChiliTimer.h"

class CApp
{
public:
	CApp();
	// ���b�Z�[�W���[�v
	int Go();
private:
	void DoFrame();
private:
	CWindow m_wnd;
	CTimer  m_timer;
};