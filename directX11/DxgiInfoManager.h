//========================================================================
// DxgiInfoManager.h
// dxgiの管理
//
// 更新日：2020/08/02
// 栗城 達也
//========================================================================
#pragma once
#include "ChiliWin.h"
#include <wrl.h>
#include <vector>
#include <dxgidebug.h>

class CDxgiInfoManager
{
public:
	CDxgiInfoManager();
	~CDxgiInfoManager() = default;
	CDxgiInfoManager(const CDxgiInfoManager&) = delete;
	CDxgiInfoManager& operator=(const CDxgiInfoManager&) = delete;
	void Set() noexcept;
	std::vector<std::string> GetMessages() const;
private:
	unsigned long long m_next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> m_pDxgiInfoQueue;
};