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
	~CApp();
private:
	void DoFrame();
private:
	CWindow m_wnd;
	CTimer  m_timer;
	std::vector < std::unique_ptr<class CBox>> m_boxes;
};