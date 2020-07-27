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
		using CChiliException::CChiliException;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	};
	class CHrException :public CException
	{
	public:
		CHrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGfxException : public CException
	{
	public:
		using CException::CException;
		const char* GetType() const noexcept override;
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
	static std::optional<int> ProcessMessages() noexcept;
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
#define CHWND_EXCEPT(hr) CWindow::CHrException(__LINE__,__FILE__,(hr))
#define CHWND_LAST_EXCEPT() CWindow::CHrException(__LINE__,__FILE__,GetLastError())
#define CHWND_NOGFX_EXCEPT() CWindow::NoGfxException(__LINE__,__FILE__)