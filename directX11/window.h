//========================================================================
// window.h
// �E�B���h�E�̍쐬�N���X
//
// �X�V���F2020/07/28
// �I�� �B��
//========================================================================
#pragma once
#include "ChiliWin.h"
#include "ChiliException.h"
#include "keyboard.h"
#include "mouse.h"
#include "Graphics.h"
#include <optional> 
#include <memory> // �X�}�[�g�|�C���^���g������


class CWindow
{
public: 
	class CException : public CChiliException // ��O�����N���X
	{
		using CChiliException::CChiliException;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept; // �E�B���h�E�̗�O�����֐�
	};
	class CHrException :public CException // HRESULT 
	{
	public:
		CHrException(int line, const char* file, HRESULT hr) noexcept; // �C���X�^���X
		const char* what() const noexcept override;                    // �G���[�R�[�h�ƃG���[�̓��e��\������
		const char* GetType() const noexcept override;                 // ��O�����̖��O
		HRESULT GetErrorCode() const noexcept;                         // hr���󂯎��
		std::string GetErrorDescription() const noexcept;              // 
	private:
		HRESULT hr;
	};
	class NoGfxException : public CException // Graphics�̗�O
	{
	public:
		using CException::CException;
		const char* GetType() const noexcept override;
	};
private:
	// �E�B���h�E�N���X�̓o�^�E�N���[���A�b�v���Ǘ�����V���O���g��(��������C���X�^���X��1�ɐ�������f�U�C���p�^�[��)
	class CWindowClass
	{
	public:
		static const char* GetName() noexcept;   // �E�B���h�E�̖��O�擾
		static HINSTANCE GetInstance() noexcept; // hInstance�擾
	private:
		CWindowClass() noexcept; //
		~CWindowClass();         //
		CWindowClass(const CWindowClass&) = delete;            // �֎~
		CWindowClass& operator=(const CWindowClass&) = delete; // �֎~
		static constexpr const char* wndClassName = "Direct3D Window"; // �E�B���h�E��
		static CWindowClass wndClass; //
		HINSTANCE hInstance;          //
	};
public:
	CWindow(int width, int height, const char* name);     // (��, ����, �E�B���h�E��)
	~CWindow();                                           //
	CWindow(const CWindow&) = delete;                     // �֎~
	CWindow& operator=(const CWindow&) = delete;          // �֎~
	void SetTitle(const std::string& title);              // �E�B���h�E���̍Đݒ�
	static std::optional<int> ProcessMessages() noexcept; // 
	CGraphics& Gfx();                                     // 
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
	std::unique_ptr<CGraphics> m_pGfx; // �X�}�[�g�|�C���^
};