#pragma once
#include "ChiliWin.h"
#include "ChiliException.h"
#include <d3d11.h>
#include <vector>
#include "DxgiInfoManager.h"

class CGraphics
{
public:
	class CException : public CChiliException
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
	class CDeviceRemoveException : public CHrException
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
	void ClearBuffer(float red, float green, float blue) noexcept;
private:
#ifndef NDEBUG
	CDxgiInfoManager infoManager;
#endif
	ID3D11Device*           m_pDevice = nullptr;
	IDXGISwapChain*         m_pSwap = nullptr;
	ID3D11DeviceContext*    m_pContext = nullptr;
	ID3D11RenderTargetView* m_pTarget = nullptr;
};