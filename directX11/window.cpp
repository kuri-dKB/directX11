//========================================================================
// window.cpp
// 
// 更新日：2020/07/28
// 栗城 達也
//========================================================================
#include "window.h"
#include <sstream>
#include "resource.h"
#include "WindowsThrowMacros.h"
#include "imgui/imgui_impl_win32.h"

// ウィンドウクラス
CWindow::CWindowClass CWindow::CWindowClass::wndClass;

CWindow::CWindowClass::CWindowClass() noexcept 
	:
	hInstance(GetModuleHandle(nullptr))
{ // ウィンドウ設定
	WNDCLASSEX wc = { 0 };       
	wc.cbSize = sizeof(wc);                       // 構造体のサイズ
	wc.style = CS_OWNDC;                          // ウィンドウスタイス
	wc.lpfnWndProc = HandleMsgSetUp;              // コールバック関数へのポインタ
	wc.cbClsExtra = 0;                            // 普通０
	wc.cbWndExtra = 0;                            // 普通０
	wc.hInstance = GetInstance();                 // インスタンス
	wc.hIcon = static_cast<HICON>(LoadImage(      // アイコン
		GetInstance(),MAKEINTRESOURCE(IDI_ICON1), //
		IMAGE_ICON, 32,32,0));                    //
	wc.hCursor = nullptr;                         // マウスカーソル
	wc.hbrBackground = nullptr;                   // ウィンドウ背景色
	wc.lpszMenuName = nullptr;                    // デフォルトメニュー名
	wc.lpszClassName = GetName();                 // ウィンドウクラス名
	wc.hIconSm = static_cast<HICON>(LoadImage(    // 16*16のアイコン
		GetInstance(), MAKEINTRESOURCE(IDI_ICON1),//
		IMAGE_ICON, 16, 16, 0));                  //
	RegisterClassEx(&wc);                         // 登録
}

CWindow::CWindowClass::~CWindowClass()
{
	UnregisterClass(wndClassName, GetInstance()); // ウィンドウ削除
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
	// ウィンドウの必要なサイズを計算。クライアント領域が指定のサイズになるように作成する
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{// エラーチェック
		throw CHWND_LAST_EXCEPT();
	}

	// ウィンドウ作成
	m_hWnd = CreateWindow(
		CWindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, CWindowClass::GetInstance(), this);
	if (m_hWnd == nullptr)
	{// エラーチェック
		throw CHWND_LAST_EXCEPT();
	}
	// 表示
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	// ImGui win32 impl初期化
	ImGui_ImplWin32_Init(m_hWnd);
	// グラフィックオブジェクト作成
	m_pGfx = std::make_unique<CGraphics>(m_hWnd);
}

CWindow::~CWindow()
{
	ImGui_ImplWin32_Shutdown();
	DestroyWindow(m_hWnd); // 削除
}

void CWindow::SetTitle(const std::string& title)
{
	if (SetWindowText(m_hWnd, title.c_str()) == 0) // ウィンドウ名変更
	{// エラーチェック
		throw CHWND_LAST_EXCEPT();
	}
}

std::optional<int> CWindow::ProcessMessages() noexcept
{
	MSG msg;
	// キューにメッセージがある間、それらを消して送る
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// WM_QUITのチェック。PeekMessageは戻り値ではシグナルを出さない
		if(msg.message == WM_QUIT)
		{ 
			// PostQuitMessageへの引数 wParamを返す
			return msg.wParam;
		}

		// msgからWM_CHARメッセージを送る
		TranslateMessage(&msg);
		DispatchMessage(&msg);		
	}

	// アプリが終了してない場合はオプションで空を返す
	return {};
}

CGraphics& CWindow::Gfx()
{
	if (!m_pGfx)
	{// エラーチェック
		throw CHWND_NOGFX_EXCEPT();
	}
	return *m_pGfx;
}

LRESULT CALLBACK CWindow::HandleMsgSetUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// CreateWindow()から渡された引数を使ってウィンドウクラスのポインタを格納
	if (msg == WM_NCCREATE)
	{
		// 作成データからウィンドウクラスへのptrを書きだす
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		CWindow* const pWnd = static_cast<CWindow*>(pCreate->lpCreateParams);
		// ウィンドウクラスにptrを格納するWinAPI管理ユーザデータを設定
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// セットアップ終了。message procをセットアップ以外のハンドルに設定
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&CWindow::HandleMsgThunk));
		// ウィンドウクラスハンドルにメッセージを送る
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// WM_NCCEATEメッセージの前にメッセージを取得したら、デフォルトのハンドルで処理
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK CWindow::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// ウィンドウクラスにptr取得
	CWindow* const pWnd = reinterpret_cast<CWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// ウィンドウクラスハンドルにメッセージを送る
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT CWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}
	const auto imio = ImGui::GetIO();

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		// ウィンドウがフォーカスを失ったときキーステートをクリアして入力を防ぐ
	case WM_KILLFOCUS:
		m_kbd.ClearState();
		break;

		//---------- KEYBOARD MESSAGES ----------//
	case WM_KEYDOWN:
		// ALTとF10を使うにはWM_SYSKEYDOWNを使わないとだめ、WM_KEYDOWNではできない
	case WM_SYSKEYDOWN:
		// imguiでキーボードを使うときこれを使わせる
		if (imio.WantCaptureKeyboard)
		{
			break;
		}
		if (!(lParam & 0x40000000) || m_kbd.AutorepeatIsEnabled()) // 自動で繰り返す
		{
			m_kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		// imguiでキーボードを使うときこれを使わせる
		if (imio.WantCaptureKeyboard)
		{
			break;
		}
		m_kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		// imguiでキーボードを使うときこれを使わせる
		if (imio.WantCaptureKeyboard)
		{
			break;
		}
		m_kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
		//---------- END KEYBOARD MESSAGE ----------//

		//---------- MOUSE MESSAGES ----------//
	case WM_MOUSEMOVE:
	{
		// imguiでマウスを使うときこれを使わせる
		if (imio.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		// クライアント領域にあるとき -> マウスの移動とキャプチャ、イベント記録
		if (pt.x >= 0 && pt.x < m_width && pt.y >= 0 && pt.y < m_height)
		{
			m_mouse.OnMouseMove(pt.x, pt.y);
			if (!m_mouse.IsInWindow())
			{
				SetCapture(hWnd);
				m_mouse.OnMouseEnter();
			}
		}
		// クライアント領域にないとき -> 移動、クリックした場合はキャプチャする
		else
		{
			if (wParam &(MK_LBUTTON | MK_RBUTTON))
			{
				m_mouse.OnMouseMove(pt.x, pt.y);
			}
			// マウス上がったとき -> リリース、イベント記録
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
		SetForegroundWindow(m_hWnd);
		// imguiでマウスを使うときこれを使わせる
		if (imio.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		// imguiでマウスを使うときこれを使わせる
		if (imio.WantCaptureMouse)
		{
			break;
		}

		const POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		// imguiでマウスを使うときこれを使わせる
		if (imio.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnLeftReleased(pt.x, pt.y);
		// ウィンドウの外に出たとき リリース
		if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height)
		{
			ReleaseCapture();
			m_mouse.OnMouseLeave();
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		// imguiでマウスを使うときこれを使わせる
		if (imio.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnRightReleased(pt.x, pt.y);
		// ウィンドウの外に出たとき リリース
		if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height)
		{
			ReleaseCapture();
			m_mouse.OnMouseLeave();
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		// imguiでマウスを使うときこれを使わせる
		if (imio.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		m_mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	//---------- END MOUSE MESSAGE ----------//
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// ウィンドウ例外処理
std::string CWindow::CException::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	// ウィンドウのエラー文字列用のメモリ確保、ポインタがそれを指すようにする
	const DWORD nMsgLen = FormatMessage( // フォーマット
		FORMAT_MESSAGE_ALLOCATE_BUFFER | // バッファの割り当てを要求
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, // GetLastError()の結果を取得できる｜変更なしでバッファに渡すよう要求
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),     // null, hr, 引数から言語識別子を作成(ユーザーのデフォルト言語)
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr               // バッファ, バッファの最小サイズ, null
	);
	if (nMsgLen == 0)
	{ // 何も入ってなかったとき、未確認のエラー
		return "Unidentified error code";
	}
	// エラーをバッファからstd::stringにコピー
	std::string errorString = pMsgBuf;
	// バッファの解放
	LocalFree(pMsgBuf);
	return errorString; 
}

CWindow::CHrException::CHrException(int line, const char* file, HRESULT hr) noexcept
	:
	CException(line, file),
	hr(hr)
{}

// エラーコードとエラーの内容を表示
const char* CWindow::CHrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl // std::uppercaseはhexとかと一緒に使われる
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()     // 16進数、大文字で表示
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl // エラーコード
		<< "[Description] " << GetErrorDescription() << std::endl                // 内容
		<< GetOriginString();                                                    // ファイル名と行
	m_whatBuffer = oss.str();    // バッファにコピー
	return m_whatBuffer.c_str(); // 返す
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