#pragma once
#include "ChiliWin.h"
#include "ChiliException.h"
#include <d3d11.h>

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
		CHrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class CDeviceRemovedException : public CHrException
	{
		using CHrException::CHrException;
	public:
		const char* GetType() const noexcept override;
	};
public:
	CGraphics(HWND hWnd);
	CGraphics(const CGraphics&) = delete;
	CGraphics& operator=(const CGraphics&) = delete;
	~CGraphics();
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
private:
	ID3D11Device* m_pDevice = nullptr;
	IDXGISwapChain* m_pSwap = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;
	ID3D11RenderTargetView* m_pTarget = nullptr;
};