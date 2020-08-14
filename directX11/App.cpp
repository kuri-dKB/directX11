//========================================================================
// App.cpp
// メイン処理
//
// 更新日：2020/08/14
// 栗城 達也
//========================================================================
#include "App.h"
#include "Box.h"
#include "Cylinder.h"
#include "Pyramid.h"
#include "SkinnedBox.h"
#include <memory>
#include <algorithm>
#include "ChiliMath.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "imgui/imgui.h"

namespace dx = DirectX;

CGDIPlusManager g_gdipm;

CApp::CApp()
	:
	m_wnd(800, 600, "実験"),
	m_light(m_wnd.Gfx())
{
	class Factory
	{
	public:
		Factory(CGraphics& gfx)
			:
			gfx(gfx)
		{}
		std::unique_ptr<CDrawable> operator()()
		{
			const DirectX::XMFLOAT3 mat = { cdist(rng),cdist(rng),cdist(rng) };
			
			switch (sdist(rng))
			{
			case 0:
				return std::make_unique<CBox>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist, mat
					);
			case 1:
				return std::make_unique<CCylinder>(
					gfx, rng, adist, ddist, odist,
					rdist, bdist, tdist
					);
			case 2:
				return std::make_unique<CPyramid>(
					gfx, rng, adist, ddist, odist,
					rdist, tdist
					);
			case 3:
				return std::make_unique<CSkinnedBox>(
					gfx, rng, adist, ddist, odist,
					rdist
					);

			default:
				assert(false && "Factoryで描画できないのがあるよ");
				return {};
			}
		}
	private:
		CGraphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_int_distribution<int> sdist{ 0, 3 };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_real_distribution<float> cdist{ 0.0f,1.0f };
		std::uniform_int_distribution<int> tdist{ 3, 30 };
	};

	m_drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(m_drawables), nDrawables, Factory{ m_wnd.Gfx() });

	// パラメータ編集用のBoxポインタを初期化
	for (auto& pd : m_drawables)
	{
		if (auto pb = dynamic_cast<CBox*>(pd.get()))
		{
			m_boxes.push_back(pb);
		}
	}

	m_wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

void CApp::DoFrame()
{
	const auto dt = m_timer.Mark() * speed_factor;
	m_wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	m_wnd.Gfx().SetCamera(m_cam.GetMatrix());
	m_light.Bind(m_wnd.Gfx(), m_cam.GetMatrix());

	for (auto& d : m_drawables)
	{
		d->Update(m_wnd.m_kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
		d->Draw(m_wnd.Gfx());
	}
	m_light.Draw(m_wnd.Gfx());

	// imgui window シュミレーションスピードコントローラー
	if (ImGui::Begin("Simulation Speed"))
	{
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 6.0f, "%.4f", 3.2f);
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Status: %s", m_wnd.m_kbd.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause)");
	}
	ImGui::End();
	// カメラ、ライトコントローラー
	m_cam.SpawnControlWindow();
	m_light.SpawnControlWindow();
	// Boxのコントローラー
	m_boxes.front()->SpawnControlerWindow(69, m_wnd.Gfx());

	// present
	m_wnd.Gfx().EndFrame();
}

CApp::~CApp()
{}


int CApp::Go()
{
	while (true)
	{
		// 保留中のメッセージを処理
		if (const auto ecode = CWindow::ProcessMessages())
		{
			// もし値があったら終了するので終了コードを返す
			return *ecode;
		}
		DoFrame();
	}
}