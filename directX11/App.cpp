//========================================================================
// App.h
// メイン処理
//
// 更新日：2020/08/07
// 栗城 達也
//========================================================================
#include "App.h"
#include "Box.h"
#include <memory>

CApp::CApp()
	:
	m_wnd(800,600, "実験")
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 80; i++)
	{
		m_boxes.push_back(std::make_unique<CBox>(
			m_wnd.Gfx(), rng, adist,
			ddist, odist, rdist
			));
	}
	m_wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

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

CApp::~CApp()
{}

void CApp::DoFrame()
{
	auto dt = m_timer.Mark();
	m_wnd.Gfx().ClearBuffer(0.07f, 0.0f, 0.12f);
	for (auto& b : m_boxes)
	{
		b->Update(dt);
		b->Draw(m_wnd.Gfx());
	}
	m_wnd.Gfx().EndFrame();
}