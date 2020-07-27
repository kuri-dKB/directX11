#include "Graphics.h"
#include "dxerr.h"
#include <sstream>

#pragma comment(lib,"d3d11.lib")

// graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) CGraphics::CHrException(__LINE__,__FILE__,(hr))
#define GFX_THROW_NOINFO(hrcall) if(FAILED(hr = (hrcall))) throw CGraphics::CHrException(__LINE__,__FILE__,hr)

#ifndef NDEBUG
#define GFX_EXCEPT(hr) CGraphics::CHrException(__LINE__,__FILE__,(hr), infoManager.GetMessages())
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if(FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVE_EXCEPT(hr) CGraphics::CDeviceRemoveException(__LINE__,__FILE__,(hr), infoManager.GetMessages())
#else
#define GFX_EXCEPT(hr) CGraphics::CHrException(__LINE__,__FILE__,(hr))
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVE_EXCEPT(hr) CGraphics::CDeviceRemoveException(__LINE__,__FILE__,(hr))
#endif


CGraphics::CGraphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// for checking results of d3d functions
	HRESULT hr;

	//  create device and front/back buffers, and swap chain and rendering context
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&m_pSwap,
		&m_pDevice,
		nullptr,
		&m_pContext
	));
	// gain access to texture subresource in swap chain (back buffer)
	ID3D11Resource* pBackBuffer = nullptr;
	GFX_THROW_INFO(m_pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer)));
	GFX_THROW_INFO(m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pTarget));
	pBackBuffer->Release();
}

CGraphics::~CGraphics()
{
	if (m_pTarget != nullptr)
	{
		m_pTarget->Release();
	}
	if (m_pContext != nullptr)
	{
		m_pContext->Release();
	}
	if (m_pSwap != nullptr)
	{
		m_pSwap->Release();
	}
	if (m_pDevice != nullptr)
	{
		m_pDevice->Release();
	}
}

void CGraphics::EndFrame()
{
	HRESULT hr;
#ifndef NDEBUG
	infoManager.Set();
#endif
	if (FAILED(hr = m_pSwap->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVE_EXCEPT(m_pDevice->GetDeviceRemovedReason());
		}
		else
		{
			throw GFX_EXCEPT(hr);
		}
	}
}

void CGraphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red,green,blue,1.0f };
	m_pContext->ClearRenderTargetView(m_pTarget, color);
}

// Graphics exception stuff
CGraphics::CHrException::CHrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	:
	CException(line, file),
	m_hr(hr)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		m_info += m;
		m_info.push_back('\n');
	}
	// remove final newline if exlists
	if (!m_info.empty())
	{
		m_info.pop_back();
	}
}

const char* CGraphics::CHrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << "(" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error string] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!m_info.empty())
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();
	m_whatBuffer = oss.str();
	return m_whatBuffer.c_str();
}

const char* CGraphics::CHrException::GetType() const noexcept
{
	return "Chili Graphics Exception";
}

HRESULT CGraphics::CHrException::GetErrorCode() const noexcept
{
	return m_hr;
}

std::string CGraphics::CHrException::GetErrorString() const noexcept
{
	return DXGetErrorString(m_hr);
}

std::string CGraphics::CHrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription(m_hr, buf, sizeof(buf));
	return buf;
}

std::string CGraphics::CHrException::GetErrorInfo() const noexcept
{
	return m_info;
}

const char* CGraphics::CDeviceRemoveException::GetType() const noexcept
{
	return "Chili Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVE)";
}