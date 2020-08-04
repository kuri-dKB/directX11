//========================================================================
// WindowsThrowMacros.h
// マクロ
//
// 更新日：2020/08/04
// 栗城 達也
//========================================================================
#pragma once

// エラー例外のヘルパー
#define CHWND_EXCEPT(hr) CWindow::CHrException(__LINE__,__FILE__,(hr))
#define CHWND_LAST_EXCEPT() CWindow::CHrException(__LINE__,__FILE__,GetLastError())
#define CHWND_NOGFX_EXCEPT() CWindow::NoGfxException(__LINE__,__FILE__)