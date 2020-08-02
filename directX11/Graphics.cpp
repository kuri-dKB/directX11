//========================================================================
// Graphics.cpp
//
// �X�V���F2020/08/02
// �I�� �B��
//========================================================================
#include "Graphics.h"
#include "dxerr.h"
#include <sstream>
#include <d3dcompiler.h>

namespace wrl = Microsoft::WRL;

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

// �O���t�B�b�N�̗�O�`�F�b�N�E�}�N��(�ꕔdxgi)
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

	// d3d�̌��ʂ��`�F�b�N����p
	HRESULT hr;

	// �f�o�C�X�A�t�����g�E�o�b�N�o�b�t�@�A�X���b�v�`�F�[���A�R���e�L�X�g�̍쐬
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
	// �X���b�v�`�F�[��(�o�b�N�o�b�t�@)�̃e�N�X�`���T�u���\�[�X�ւ̃p�X���Ƃ�
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

void CGraphics::DrawTextTriangle()
{
	namespace wrl = Microsoft::WRL;
	HRESULT hr;

	struct Vertex
	{
		float x;
		float y;
	};

	// ���_�o�b�t�@�̍쐬(�O�p�`)
	const Vertex vertices[] =
	{
		{ 0.0f,  0.5f },
		{ 0.5f, -0.5f },
		{ -0.5f, -0.5f },
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

	// ���_�o�b�t�@���Z�b�g
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	m_pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);


	// �s�N�Z���V�F�[�_�[�̍쐬
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(L"PixcelShader.cso", &pBlob));
	GFX_THROW_INFO(m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	// �Z�b�g
	m_pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);


	// �o�[�e�b�N�X�V�F�[�_�[�̍쐬
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	GFX_THROW_INFO(m_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

	// �Z�b�g
	m_pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);


	// �C���v�b�g���C�A�E�g(�QD�ʒu�̂�)
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	GFX_THROW_INFO(m_pDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	));

	// �Z�b�g
	m_pContext->IASetInputLayout(pInputLayout.Get());


	// �����_�[�^�[�Q�b�g���Z�b�g
	m_pContext->OMSetRenderTargets(1u, m_pTarget.GetAddressOf(), nullptr);


	// �v���~�e�B�u�ݒ���g���C�A���O�����X�g�ɃZ�b�g
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pContext->RSSetViewports(1u, &vp);

	GFX_THROW_INFO_ONLY(m_pContext->Draw((UINT)std::size(vertices), 0u));
}


// ��O����
CGraphics::CHrException::CHrException(int line, const char * file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	:
	CException(line, file),
	m_hr(hr)
{
	// �S�Ă� infoMsgs �����s���� m_info�ɓ����
	for (const auto& m : infoMsgs)
	{
		m_info += m;
		m_info.push_back('\n');
	}
	// �Ō�̉��s���폜
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
	// �S�Ă� infoMsgs �����s���� m_info�ɓ����
	for (const auto& m : infoMsgs)
	{
		m_info += m;
		m_info.push_back('\n');
	}
	// �Ō�̉��s���폜
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


