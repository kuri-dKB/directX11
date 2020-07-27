//========================================================================
// window.cpp
// 
//
// 栗城 達也
//========================================================================
#include "window.h"
//#include "windowsmessagemap.h"
#include <sstream>
#include "resource.h"

// ウィンドウクラス
CWindow::CWindowClass CWindow::CWindowClass::wndClass;

CWindow::CWindowClass::CWindowClass() noexcept 
	:
	hInstance(GetModuleHandle(nullptr))
{ // ウィンドウ設定
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetUp;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(
		GetInstance(),MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON, 32,32,0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(
		GetInstance(), MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON, 16, 16, 0));
	RegisterClassEx(&wc);
}

CWindow::CWindowClass::~CWindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}


const char* CWindow::CWindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE CWindow::CWindowClass::GetInstance() noexcept
{
	return wndClass.hInstance;
}

// ウィンドウ作成
CWindow::CWindow(int width, int height, const char* name) 
	:
	m_width(width),
	m_height(height)
{
	// 引数のウィンドウサイズに合わせて計算
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw CHWND_LAST_EXCEPT();
	}

	// ウィンドウ作成
	m_hWnd = CreateWindow(
		CWindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, CWindowClass::GetInstance(), this);
	// エラーチェック
	if (m_hWnd == nullptr)
	{
		throw CHWND_LAST_EXCEPT();
	}
	// newly created windows start off as hidden
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	// create graphics object
	m_pGfx = std::make_unique<CGraphics>(m_hWnd);
}

CWindow::~CWindow()
{
	DestroyWindow(m_hWnd);
}

void CWindow::SetTitle(const std::string& title)
{
	if (SetWindowText(m_hWnd, title.c_str()) == 0)
	{
		throw CHWND_LAST_EXCEPT();
	}
}

std::optional<int> CWindow::ProcessMessages() noexcept
{
	MSG msg;
	// while queue has messages, remove and dispatch them
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// check for quit because peekmessage does not signal this via return
		if(msg.message == WM_QUIT)
		{ 
			// return optional wrapping int 
			return msg.wParam;
		}

		// TranslateMesssage will post auxilliary WM_CHAR messages from key msg
		TranslateMessage(&msg);
		DispatchMessage(&msg);		
	}

	// return empty optional when not quitting app
	return {};
}

CGraphics& CWindow::Gfx()
{
	if (!m_pGfx)
	{
		throw CHWND_NOGFX_EXCEPT();
	}
	return *m_pGfx;
}

LRESULT CALLBACK CWindow::HandleMsgSetUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// CreateWindow()から渡された引数を使ってウィンドウクラスのポインタを格納
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		CWindow* const pWnd = static_cast<CWindow*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&CWindow::HandleMsgThunk));
		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK CWindow::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// retrieve ptr to window class
	CWindow* const pWnd = reinterpret_cast<CWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT CWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	// we don't want the DefProc to handle this message because
	// we want our destructor to destoroy the window, so return 0 instead of break
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		// clear keystate when window loses focus to prevent input getting
	case WM_KILLFOCUS:
		m_kbd.ClearState();
		break;

		//---------- KEYBOARD MESSAGES
	case WM_KEYDOWN:
		// syskey commads need to be handle to track ALT key (VK_MENU) and F10
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || m_kbd.AutorepeatIsEnabled()) // filter autorepeat
		{
			m_kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		m_kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		m_kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
		//---------- END KEYBOARD MESSAGE

		//__________ MOUSE MESSAGES
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		// in client region -> log move, and log enter + capture mouse
		if (pt.x >= 0 && pt.x < m_width && pt.y >= 0 && pt.y < m_height)
		{
			m_mouse.OnMouseMove(pt.x, pt.y);
			if (!m_mouse.IsInWindow())
			{
				SetCapture(hWnd);
				m_mouse.OnMouseEnter();
			}
		}
		// not in client -> log move / maintain capture if button down
		else
		{
			if (wParam &(MK_LBUTTON | MK_RBUTTON))
			{
				m_mouse.OnMouseMove(pt.x, pt.y);
			}
			// button up -> release capture / log event for leaving
			else
			{
				ReleaseCapture();
				m_mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		m_mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	//---------- END MOUSE MESSAGE
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Window Exception Stuff
std::string CWindow::CException::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	// window will allocate memory for err string and make our pointer point to it
	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;
	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}

CWindow::CHrException::CHrException(int line, const char* file, HRESULT hr) noexcept
	:
	CException(line, file),
	hr(hr)
{}

const char* CWindow::CHrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	m_whatBuffer = oss.str();
	return m_whatBuffer.c_str();
}

const char* CWindow::CHrException::GetType() const noexcept
{
	return "Chili Window Exception";
}

HRESULT CWindow::CHrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string CWindow::CHrException::GetErrorDescription() const noexcept
{
	return CException::TranslateErrorCode(hr);
}

const char* CWindow::NoGfxException::GetType() const noexcept
{
	return "Chili Window Exception [No Graphics]";
}