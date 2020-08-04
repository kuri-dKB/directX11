//========================================================================
// Graphics.cpp
//
// 更新日：2020/08/02
// 栗城 達也
//========================================================================
#include "Graphics.h"
#include "dxerr.h"
#include <sstream>
#include <d3dcompiler.h>
#include <cmath>
#include <DirectXMath.h>

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;


#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

// グラフィックの例外チェック・マクロ(一部dxgi)
#define GFX_EXCEPT_NOINFO(hr) CGraphics::CHrException(__LINE__,__FILE__,(hr))
#define GXF_THROW_NOINFO(hrcall) if(FAILED(hr = (hrcall))) throw CGraphics::CHrException(__LINE__,__FILE__,hr)

#ifndef NDEBUG
#define GFX_EXCEPT(hr) CGraphics::CHrException( __LINE__,__FILE__,(hr),m_infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) m_infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) CGraphics::CDeviceRemovedException( __LINE__,__FILE__,(hr),m_infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) m_infoManager.Set(); (call); {auto v = m_infoManager.GetMessages(); if(!v.empty()) {throw CGraphics::CInfoException(__LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) CGraphics::CHrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) CGraphics::CDeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif // !NDEBUG


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
	
}

void CGraphics::EndFrame()
{
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

void CGraphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red,green,blue,1.0f };
	m_pContext->ClearRenderTargetView(m_pTarget.Get(), color);
}

void CGraphics::DrawTextTriangle(float angle, float x, float z)
{
	HRESULT hr;

	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		}pos;
	};

	// 頂点バッファの作成
	Vertex vertices[] =
	{
		{ -1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ -1.0f, 1.0f, -1.0f },
		{ 1.0f, 1.0f, -1.0f },
		{ -1.0f, -1.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
	};

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	GFX_THROW_INFO(m_pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

	// 頂点バッファをセット
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	m_pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);


	// インデックスバッファ作成
	const unsigned short indices[] =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4,
	};
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	GFX_THROW_INFO(m_pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

	// インデックスバッファをセット
	m_pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);


	// コンスタントバッファ作成
	struct ConstantBuffer
	{
		dx::XMMATRIX transform;
	};
	const ConstantBuffer cb =
	{
		{
			dx::XMMatrixTranspose
			(
				dx::XMMatrixRotationZ(angle) *
				dx::XMMatrixRotationX(angle) *
				dx::XMMatrixTranslation(x, 0.0f, z + 4.0f) *
				dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
			)
			
		}
	};
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	GFX_THROW_INFO(m_pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

	// コンスタントバッファセット
	m_pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	// lookup table for cube face colors
	struct ConstantBuffer2
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	const ConstantBuffer2 cb2 =
	{
		{
			{1.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 1.0f},
		}
	};
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer2;
	D3D11_BUFFER_DESC cbd2;
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(cb2);
	cbd2.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd2 = {};
	csd2.pSysMem = &cb2;
	GFX_THROW_INFO(m_pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));

	// set
	m_pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());


	// ピクセルシェーダーの作成
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(L"PixcelShader.cso", &pBlob));
	GFX_THROW_INFO(m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	// セット
	m_pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);


	// バーテックスシェーダーの作成
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	GFX_THROW_INFO(m_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

	// セット
	m_pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);


	// インプットレイアウト(２D位置のみ)
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	GFX_THROW_INFO(m_pDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	));

	// セット
	m_pContext->IASetInputLayout(pInputLayout.Get());


	// レンダーターゲットをセット
	m_pContext->OMSetRenderTargets(1u, m_pTarget.GetAddressOf(), nullptr);


	// プリミティブ設定をトライアングルリストにセット
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pContext->RSSetViewports(1u, &vp);

	GFX_THROW_INFO_ONLY(m_pContext->DrawIndexed((UINT)std::size(indices),0u, 0u));
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


