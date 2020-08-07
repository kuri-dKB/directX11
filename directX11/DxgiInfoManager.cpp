//========================================================================
// DxgiInfoManager.cpp
// dxgiの管理
//
// 更新日：2020/08/02
// 栗城 達也
//========================================================================
#include "DxgiInfoManager.h"
#include "window.h"
#include "Graphics.h"
#include <dxgidebug.h>
#include <memory>
#include "GraphicsThrowMacros.h"
#include "WindowsThrowMacros.h"

#pragma comment(lib, "dxguid.lib")


CDxgiInfoManager::CDxgiInfoManager()
{
	// DXGIGetDebugInterfaceのシグネチャを定義
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	// DXGIGetDebugInterface()を含むdllをロード
	const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDxgiDebug == nullptr)
	{
		throw CHWND_LAST_EXCEPT();
	}

	// dllでDXGIGetDebugInterface()のアドレスを取得
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
		);
	if (DxgiGetDebugInterface == nullptr)
	{
		throw CHWND_LAST_EXCEPT();
	}

	HRESULT hr;
	GFX_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &m_pDxgiInfoQueue));
}

void CDxgiInfoManager::Set() noexcept
{
	// エラーを取得するようインデックス(m_next)を設定
	m_next = m_pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> CDxgiInfoManager::GetMessages() const
{
	std::vector<std::string> messages;
	const auto end = m_pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = m_next; i < end; i++)
	{
		HRESULT hr;
		SIZE_T messageLength;
		// i のサイズをバイト数で取得
		GFX_THROW_NOINFO(m_pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
		// メッセージ用のメモリを割り当てる
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		// メッセージを取得してベクトルにプッシュする
		GFX_THROW_NOINFO(m_pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		messages.emplace_back(pMessage->pDescription);
	}
	return messages;
}

