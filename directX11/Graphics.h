//========================================================================
// Graphics.h
// 描画系、例外処理
//
// 更新日：2020/08/11
// 栗城 達也
//========================================================================
#pragma once
#include "ChiliWin.h"
#include "ChiliException.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "DxgiInfoManager.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>

class CGraphics
{
	friend class CBindable;
public:
	class CException : public CChiliException // 例外
	{
		using CChiliException::CChiliException;
	};
	class CHrException : public CException
	{
	public:
		CHrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT m_hr;
		std::string m_info;
	};
	class CInfoException : public CException
	{
	public:
		CInfoException(int line, const char* file, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string m_info;
	};
	class CDeviceRemovedException : public CHrException
	{
		using CHrException::CHrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string m_reason;
	};
public:
	CGraphics(HWND hWnd);
	CGraphics(const CGraphics&) = delete;
	CGraphics& operator=(const CGraphics&) = delete;
	~CGraphics();
	void EndFrame();
	void BeginFrame(float red, float green, float blue) noexcept;
	void DrawIndexed(UINT count) noexcept(!IS_DEBUG);
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	void SetCamera(DirectX::FXMMATRIX cam) noexcept;
	DirectX::XMMATRIX GetCamera() const noexcept;
	void EnableImgui() noexcept;
	void DisableImgui() noexcept;
	bool IsImguiEnabled() const noexcept;
private:
	DirectX::XMMATRIX m_projection;
	DirectX::XMMATRIX m_camera;
	bool m_imguiEnabled = true;
#ifndef NDEBUG
	CDxgiInfoManager m_infoManager;
#endif // !NDEBUG
	Microsoft::WRL::ComPtr<ID3D11Device>           m_pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain>         m_pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>    m_pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDSV;
};