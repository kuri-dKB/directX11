//========================================================================
// App.cpp
// メイン処理
//
// 更新日：2020/08/09
// 栗城 達也
//========================================================================
#include "App.h"
#include "Melon.h"
#include "Pyramid.h"
#include "Box.h"
#include <memory>
#include <algorithm>
#include "ChiliMath.h"


CApp::CApp()
	:
	m_wnd(800, 600, "実験")
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
			switch (typedist(rng))
			{
			case 0:
				return std::make_unique<CPyramid>(
					gfx, rng, adist, ddist,
					odist, rdist
					);
			case 1:
				return std::make_unique<CBox>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist
					);
			case 2:
				return std::make_unique<CMelon>(
					gfx, rng, adist, ddist,
					odist, rdist, longdist, latdist
					);
			default:
				assert(false && "ファクトリのタイプとあってないよ");
				return {};
			}
		}
	private:
		CGraphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_int_distribution<int> latdist{ 5,20 };
		std::uniform_int_distribution<int> longdist{ 10,40 };
		std::uniform_int_distribution<int> typedist{ 0,2 };
	};

	Factory f(m_wnd.Gfx());
	m_drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(m_drawables), nDrawables, f);

	m_wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

void CApp::DoFrame()
{
	const auto dt = m_timer.Mark();
	m_wnd.Gfx().ClearBuffer(0.07f, 0.0f, 0.12f);
	for (auto& d : m_drawables)
	{
		d->Update(dt);
		d->Draw(m_wnd.Gfx());
	}
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