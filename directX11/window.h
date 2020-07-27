//========================================================================
// window.h
// ウィンドウの作成クラス
//
// 栗城 達也
//========================================================================
#pragma once
#include "ChiliWin.h"
#include "ChiliException.h"
#include "keyboard.h"
#include "mouse.h"
#include "Graphics.h"
#include <optional>
#include <memory>

class CWindow
{
public:
	class CException : public CChiliException
	{
	public:
		CException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT m_hr;
	};
private:
	// シングルトン
	class CWindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		CWindowClass() noexcept;
		~CWindowClass();
		CWindowClass(const CWindowClass&) = delete;
		CWindowClass& operator=(const CWindowClass&) = delete;
		static constexpr const char* wndClassName = "Direct3D Window";
		static CWindowClass wndClass;
		HINSTANCE hInstance;
	};
public:
	CWindow(int width, int height, const char* name);
	~CWindow();
	CWindow(const CWindow&) = delete;
	CWindow& operator=(const CWindow&) = delete;
	void SetTitle(const std::string& title);
	static std::optional<int> ProcessMessages();
	CGraphics& Gfx();
private:
	static LRESULT CALLBACK HandleMsgSetUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	CKeyboard m_kbd;
	CMouse    m_mouse;
private:
	int  m_width;
	int  m_height;
	HWND m_hWnd;
	std::unique_ptr<CGraphics> m_pGfx;
};


// error exception helper macro
#define CHWND_EXCEPT(hr) CWindow::CException(__LINE__,__FILE__,hr)
#define CHWND_LAST_EXCEPT() CWindow::CException(__LINE__,__FILE__,GetLastError())