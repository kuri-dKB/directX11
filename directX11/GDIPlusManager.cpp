//========================================================================
// GDIPlusManger.cpp
// 
//
// 更新日：2020/08/10
// 栗城 達也
//========================================================================
#define FULL_WINTARD
#include "ChiliWin.h"
#include "GDIPlusManager.h"
#include <algorithm>
namespace Gdiplus
{
	using std::min;
	using std::max;
}
#include <gdiplus.h>

ULONG_PTR CGDIPlusManager::m_token = 0;
int CGDIPlusManager::m_refCount = 0;

CGDIPlusManager::CGDIPlusManager()
{
	if (m_refCount++ == 0)
	{
		Gdiplus::GdiplusStartupInput input;
		input.GdiplusVersion = 1;
		input.DebugEventCallback = nullptr;
		input.SuppressBackgroundThread = false;
		Gdiplus::GdiplusStartup(&m_token, &input, nullptr);
	}
}

CGDIPlusManager::~CGDIPlusManager()
{
	if (--m_refCount == 0)
	{
		Gdiplus::GdiplusShutdown(m_token);
	}
}