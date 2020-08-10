//========================================================================
// window.cpp
// 
// �X�V���F2020/07/28
// �I�� �B��
//========================================================================
#include "window.h"
#include <sstream>
#include "resource.h"
#include "WindowsThrowMacros.h"
#include "imgui/imgui_impl_win32.h"

// �E�B���h�E�N���X
CWindow::CWindowClass CWindow::CWindowClass::wndClass;

CWindow::CWindowClass::CWindowClass() noexcept 
	:
	hInstance(GetModuleHandle(nullptr))
{ // �E�B���h�E�ݒ�
	WNDCLASSEX wc = { 0 };       
	wc.cbSize = sizeof(wc);                       // �\���̂̃T�C�Y
	wc.style = CS_OWNDC;                          // �E�B���h�E�X�^�C�X
	wc.lpfnWndProc = HandleMsgSetUp;              // �R�[���o�b�N�֐��ւ̃|�C���^
	wc.cbClsExtra = 0;                            // ���ʂO
	wc.cbWndExtra = 0;                            // ���ʂO
	wc.hInstance = GetInstance();                 // �C���X�^���X
	wc.hIcon = static_cast<HICON>(LoadImage(      // �A�C�R��
		GetInstance(),MAKEINTRESOURCE(IDI_ICON1), //
		IMAGE_ICON, 32,32,0));                    //
	wc.hCursor = nullptr;                         // �}�E�X�J�[�\��
	wc.hbrBackground = nullptr;                   // �E�B���h�E�w�i�F
	wc.lpszMenuName = nullptr;                    // �f�t�H���g���j���[��
	wc.lpszClassName = GetName();                 // �E�B���h�E�N���X��
	wc.hIconSm = static_cast<HICON>(LoadImage(    // 16*16�̃A�C�R��
		GetInstance(), MAKEINTRESOURCE(IDI_ICON1),//
		IMAGE_ICON, 16, 16, 0));                  //
	RegisterClassEx(&wc);                         // �o�^
}

CWindow::CWindowClass::~CWindowClass()
{
	UnregisterClass(wndClassName, GetInstance()); // �E�B���h�E�폜
}


const char* CWindow::CWindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE CWindow::CWindowClass::GetInstance() noexcept
{
	return wndClass.hInstance;
}

// �E�B���h�E�쐬
CWindow::CWindow(int width, int height, const char* name) 
	:
	m_width(width),
	m_height(height)
{
	// �����̃E�B���h�E�T�C�Y�ɍ��킹�Čv�Z
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	// �E�B���h�E�̕K�v�ȃT�C�Y���v�Z�B�N���C�A���g�̈悪�w��̃T�C�Y�ɂȂ�悤�ɍ쐬����
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{// �G���[�`�F�b�N
		throw CHWND_LAST_EXCEPT();
	}

	// �E�B���h�E�쐬
	m_hWnd = CreateWindow(
		CWindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, CWindowClass::GetInstance(), this);
	if (m_hWnd == nullptr)
	{// �G���[�`�F�b�N
		throw CHWND_LAST_EXCEPT();
	}
	// �\��
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	// ImGui win32 impl������
	ImGui_ImplWin32_Init(m_hWnd);
	// �O���t�B�b�N�I�u�W�F�N�g�쐬
	m_pGfx = std::make_unique<CGraphics>(m_hWnd);
}

CWindow::~CWindow()
{
	ImGui_ImplWin32_Shutdown();
	DestroyWindow(m_hWnd); // �폜
}

void CWindow::SetTitle(const std::string& title)
{
	if (SetWindowText(m_hWnd, title.c_str()) == 0) // �E�B���h�E���ύX
	{// �G���[�`�F�b�N
		throw CHWND_LAST_EXCEPT();
	}
}

std::optional<int> CWindow::ProcessMessages() noexcept
{
	MSG msg;
	// �L���[�Ƀ��b�Z�[�W������ԁA�����������đ���
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// WM_QUIT�̃`�F�b�N�BPeekMessage�͖߂�l�ł̓V�O�i�����o���Ȃ�
		if(msg.message == WM_QUIT)
		{ 
			// PostQuitMessage�ւ̈��� wParam��Ԃ�
			return msg.wParam;
		}

		// msg����WM_CHAR���b�Z�[�W�𑗂�
		TranslateMessage(&msg);
		DispatchMessage(&msg);		
	}

	// �A�v�����I�����ĂȂ��ꍇ�̓I�v�V�����ŋ��Ԃ�
	return {};
}

CGraphics& CWindow::Gfx()
{
	if (!m_pGfx)
	{// �G���[�`�F�b�N
		throw CHWND_NOGFX_EXCEPT();
	}
	return *m_pGfx;
}

LRESULT CALLBACK CWindow::HandleMsgSetUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// CreateWindow()����n���ꂽ�������g���ăE�B���h�E�N���X�̃|�C���^���i�[
	if (msg == WM_NCCREATE)
	{
		// �쐬�f�[�^����E�B���h�E�N���X�ւ�ptr����������
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		CWindow* const pWnd = static_cast<CWindow*>(pCreate->lpCreateParams);
		// �E�B���h�E�N���X��ptr���i�[����WinAPI�Ǘ����[�U�f�[�^��ݒ�
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// �Z�b�g�A�b�v�I���Bmessage proc���Z�b�g�A�b�v�ȊO�̃n���h���ɐݒ�
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&CWindow::HandleMsgThunk));
		// �E�B���h�E�N���X�n���h���Ƀ��b�Z�[�W�𑗂�
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// WM_NCCEATE���b�Z�[�W�̑O�Ƀ��b�Z�[�W���擾������A�f�t�H���g�̃n���h���ŏ���
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK CWindow::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// �E�B���h�E�N���X��ptr�擾
	CWindow* const pWnd = reinterpret_cast<CWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// �E�B���h�E�N���X�n���h���Ƀ��b�Z�[�W�𑗂�
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
		// �E�B���h�E���t�H�[�J�X���������Ƃ��L�[�X�e�[�g���N���A���ē��͂�h��
	case WM_KILLFOCUS:
		m_kbd.ClearState();
		break;

		//---------- KEYBOARD MESSAGES ----------//
	case WM_KEYDOWN:
		// ALT��F10���g���ɂ�WM_SYSKEYDOWN���g��Ȃ��Ƃ��߁AWM_KEYDOWN�ł͂ł��Ȃ�
	case WM_SYSKEYDOWN:
		// imgui�ŃL�[�{�[�h���g���Ƃ�������g�킹��
		if (imio.WantCaptureKeyboard)
		{
			break;
		}
		if (!(lParam & 0x40000000) || m_kbd.AutorepeatIsEnabled()) // �����ŌJ��Ԃ�
		{
			m_kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		// imgui�ŃL�[�{�[�h���g���Ƃ�������g�킹��
		if (imio.WantCaptureKeyboard)
		{
			break;
		}
		m_kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		// imgui�ŃL�[�{�[�h���g���Ƃ�������g�킹��
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
		// imgui�Ń}�E�X���g���Ƃ�������g�킹��
		if (imio.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		// �N���C�A���g�̈�ɂ���Ƃ� -> �}�E�X�̈ړ��ƃL���v�`���A�C�x���g�L�^
		if (pt.x >= 0 && pt.x < m_width && pt.y >= 0 && pt.y < m_height)
		{
			m_mouse.OnMouseMove(pt.x, pt.y);
			if (!m_mouse.IsInWindow())
			{
				SetCapture(hWnd);
				m_mouse.OnMouseEnter();
			}
		}
		// �N���C�A���g�̈�ɂȂ��Ƃ� -> �ړ��A�N���b�N�����ꍇ�̓L���v�`������
		else
		{
			if (wParam &(MK_LBUTTON | MK_RBUTTON))
			{
				m_mouse.OnMouseMove(pt.x, pt.y);
			}
			// �}�E�X�オ�����Ƃ� -> �����[�X�A�C�x���g�L�^
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
		// imgui�Ń}�E�X���g���Ƃ�������g�킹��
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
		// imgui�Ń}�E�X���g���Ƃ�������g�킹��
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
		// imgui�Ń}�E�X���g���Ƃ�������g�킹��
		if (imio.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnLeftReleased(pt.x, pt.y);
		// �E�B���h�E�̊O�ɏo���Ƃ� �����[�X
		if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height)
		{
			ReleaseCapture();
			m_mouse.OnMouseLeave();
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		// imgui�Ń}�E�X���g���Ƃ�������g�킹��
		if (imio.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnRightReleased(pt.x, pt.y);
		// �E�B���h�E�̊O�ɏo���Ƃ� �����[�X
		if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height)
		{
			ReleaseCapture();
			m_mouse.OnMouseLeave();
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		// imgui�Ń}�E�X���g���Ƃ�������g�킹��
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

// �E�B���h�E��O����
std::string CWindow::CException::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	// �E�B���h�E�̃G���[������p�̃������m�ہA�|�C���^��������w���悤�ɂ���
	const DWORD nMsgLen = FormatMessage( // �t�H�[�}�b�g
		FORMAT_MESSAGE_ALLOCATE_BUFFER | // �o�b�t�@�̊��蓖�Ă�v��
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, // GetLastError()�̌��ʂ��擾�ł���b�ύX�Ȃ��Ńo�b�t�@�ɓn���悤�v��
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),     // null, hr, �������猾�ꎯ�ʎq���쐬(���[�U�[�̃f�t�H���g����)
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr               // �o�b�t�@, �o�b�t�@�̍ŏ��T�C�Y, null
	);
	if (nMsgLen == 0)
	{ // ���������ĂȂ������Ƃ��A���m�F�̃G���[
		return "Unidentified error code";
	}
	// �G���[���o�b�t�@����std::string�ɃR�s�[
	std::string errorString = pMsgBuf;
	// �o�b�t�@�̉��
	LocalFree(pMsgBuf);
	return errorString; 
}

CWindow::CHrException::CHrException(int line, const char* file, HRESULT hr) noexcept
	:
	CException(line, file),
	hr(hr)
{}

// �G���[�R�[�h�ƃG���[�̓��e��\��
const char* CWindow::CHrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl // std::uppercase��hex�Ƃ��ƈꏏ�Ɏg����
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()     // 16�i���A�啶���ŕ\��
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl // �G���[�R�[�h
		<< "[Description] " << GetErrorDescription() << std::endl                // ���e
		<< GetOriginString();                                                    // �t�@�C�����ƍs
	m_whatBuffer = oss.str();    // �o�b�t�@�ɃR�s�[
	return m_whatBuffer.c_str(); // �Ԃ�
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