//========================================================================
// window.h
// ウィンドウの作成クラス
//
// 更新日：2020/07/28
// 栗城 達也
//========================================================================
#pragma once
#include "ChiliWin.h"
#include "ChiliException.h"
#include "keyboard.h"
#include "mouse.h"
#include "Graphics.h"
#include <optional> 
#include <memory> // スマートポインタを使うため


class CWindow
{
public: 
	class CException : public CChiliException // 例外処理クラス
	{
		using CChiliException::CChiliException;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept; // ウィンドウの例外処理関数
	};
	class CHrException :public CException // HRESULT 
	{
	public:
		CHrException(int line, const char* file, HRESULT hr) noexcept; // インスタンス
		const char* what() const noexcept override;                    // エラーコードとエラーの内容を表示する
		const char* GetType() const noexcept override;                 // 例外処理の名前
		HRESULT GetErrorCode() const noexcept;                         // hrを受け取る
		std::string GetErrorDescription() const noexcept;              // 
	private:
		HRESULT hr;
	};
	class NoGfxException : public CException // Graphicsの例外
	{
	public:
		using CException::CException;
		const char* GetType() const noexcept override;
	};
private:
	// ウィンドウクラスの登録・クリーンアップを管理するシングルトン(生成するインスタンスを1つに制限するデザインパターン)
	class CWindowClass
	{
	public:
		static const char* GetName() noexcept;   // ウィンドウの名前取得
		static HINSTANCE GetInstance() noexcept; // hInstance取得
	private:
		CWindowClass() noexcept; //
		~CWindowClass();         //
		CWindowClass(const CWindowClass&) = delete;            // 禁止
		CWindowClass& operator=(const CWindowClass&) = delete; // 禁止
		static constexpr const char* wndClassName = "Direct3D Window"; // ウィンドウ名
		static CWindowClass wndClass; //
		HINSTANCE hInstance;          //
	};
public:
	CWindow(int width, int height, const char* name);     // (幅, 高さ, ウィンドウ名)
	~CWindow();                                           //
	CWindow(const CWindow&) = delete;                     // 禁止
	CWindow& operator=(const CWindow&) = delete;          // 禁止
	void SetTitle(const std::string& title);              // ウィンドウ名の再設定
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
	std::unique_ptr<CGraphics> m_pGfx; // スマートポインタ
};