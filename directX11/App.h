//========================================================================
// App.h
// ���C������
//
// �X�V���F2020/08/14
// �I�� �B��
//========================================================================
#pragma once
#include "window.h"
#include "ChiliTimer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include <set>

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
	std::vector<class CBox*> m_boxes;
	float speed_factor = 1.0f;
	CCamera m_cam;
	CPointLight m_light;
	static constexpr size_t nDrawables = 180; // ���f���̌�
	std::optional<int> m_comboBoxIndex;
	std::set<int> m_boxControlIds;
};