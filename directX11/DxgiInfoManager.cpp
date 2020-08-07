//========================================================================
// DxgiInfoManager.cpp
// dxgi�̊Ǘ�
//
// �X�V���F2020/08/02
// �I�� �B��
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
	// DXGIGetDebugInterface�̃V�O�l�`�����`
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	// DXGIGetDebugInterface()���܂�dll�����[�h
	const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDxgiDebug == nullptr)
	{
		throw CHWND_LAST_EXCEPT();
	}

	// dll��DXGIGetDebugInterface()�̃A�h���X���擾
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
	// �G���[���擾����悤�C���f�b�N�X(m_next)��ݒ�
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
		// i �̃T�C�Y���o�C�g���Ŏ擾
		GFX_THROW_NOINFO(m_pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
		// ���b�Z�[�W�p�̃����������蓖�Ă�
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		// ���b�Z�[�W���擾���ăx�N�g���Ƀv�b�V������
		GFX_THROW_NOINFO(m_pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		messages.emplace_back(pMessage->pDescription);
	}
	return messages;
}

