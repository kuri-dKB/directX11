//========================================================================
// Graphics.cpp
//
// 更新日：2020/08/10
// 栗城 達也
//========================================================================
#include "Graphics.h"
#include "dxerr.h"
#include <sstream>
#include <d3dcompiler.h>
#include <cmath>
#include <DirectXMath.h>
#include "GraphicsThrowMacros.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")


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
#endif // !NDEBUG

	// d3dの結果をチェックする用
	HRESULT hr;

	// デバイス、フロント・バックバッファ、スワップチェーン、コンテキストの作成
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
	// スワップチェーン(バックバッファ)のテクスチャサブリソースへのパスをとる
	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	GFX_THROW_INFO(m_pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	GFX_THROW_INFO(m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pTarget));
	
	// デプスステンシルステート作成
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	GFX_THROW_INFO(m_pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

	// セット
	m_pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	// デプスステンシルテクスチャ作成
	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 800u;
	descDepth.Height = 600u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_THROW_INFO(m_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	// ビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	GFX_THROW_INFO(m_pDevice->CreateDepthStencilView(
		pDepthStencil.Get(), &descDSV, &m_pDSV
	));

	// セット
	m_pContext->OMSetRenderTargets(1u, m_pTarget.GetAddressOf(), m_pDSV.Get());

	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = 800.0f;
	vp.Height = 600.0f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	m_pContext->RSSetViewports(1u, &vp);

	// imgui d3d impl 初期化
	ImGui_ImplDX11_Init(m_pDevice.Get(), m_pContext.Get());
}

void CGraphics::EndFrame()
{
	// imgui frame end
	if (m_imguiEnabled)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	HRESULT hr;
#ifndef NDEBUG
	m_infoManager.Set();
#endif // !NDEBUG
	if (FAILED(hr = m_pSwap->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(m_pDevice->GetDeviceRemovedReason());
		}
		else
		{
			throw GFX_EXCEPT(hr);
		}
	}
}

void CGraphics::BeginFrame(float red, float green, float blue) noexcept
{
	// imgui begin frame
	if (m_imguiEnabled)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	const float color[] = { red,green,blue,1.0f };
	m_pContext->ClearRenderTargetView(m_pTarget.Get(), color);
	m_pContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void CGraphics::DrawIndexed(UINT count) noexcept(!IS_DEBUG)
{
	GFX_THROW_INFO_ONLY(m_pContext->DrawIndexed(count, 0u, 0u));
}

void CGraphics::SetProjection(DirectX::FXMMATRIX proj) noexcept
{
	m_projection = proj;
}

DirectX::XMMATRIX CGraphics::GetProjection() const noexcept
{
	return m_projection;
}

void CGraphics::SetCamera(DirectX::FXMMATRIX cam) noexcept
{
	m_camera = cam;
}

DirectX::XMMATRIX CGraphics::GetCamera() const noexcept
{
	return m_camera;
}

void CGraphics::EnableImgui() noexcept
{
	m_imguiEnabled = true;
}

void CGraphics::DisableImgui() noexcept
{
	m_imguiEnabled = false;
}

bool CGraphics::IsImguiEnabled() const noexcept
{
	return m_imguiEnabled;
}

// 例外処理
CGraphics::CHrException::CHrException(int line, const char * file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	:
	CException(line, file),
	m_hr(hr)
{
	// 全ての infoMsgs を改行して m_infoに入れる
	for (const auto& m : infoMsgs)
	{
		m_info += m;
		m_info.push_back('\n');
	}
	// 最後の改行を削除
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
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
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

const char* CGraphics::CDeviceRemovedException::GetType() const noexcept
{
	return "Chili Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

CGraphics::CInfoException::CInfoException(int line, const char * file, std::vector<std::string> infoMsgs) noexcept
	:
	CException(line, file)
{
	// 全ての infoMsgs を改行して m_infoに入れる
	for (const auto& m : infoMsgs)
	{
		m_info += m;
		m_info.push_back('\n');
	}
	// 最後の改行を削除
	if (!m_info.empty())
	{
		m_info.pop_back();
	}
}

const char* CGraphics::CInfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	m_whatBuffer = oss.str();
	return m_whatBuffer.c_str();
}

const char* CGraphics::CInfoException::GetType() const noexcept
{
	return "Chili Graphics Info Exception";
}

std::string CGraphics::CInfoException::GetErrorInfo() const noexcept
{
	return m_info;
}


