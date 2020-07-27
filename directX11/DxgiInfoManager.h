#pragma once
#include "ChiliWin.h"
#include <vector>

class CDxgiInfoManager
{
public:
	CDxgiInfoManager();
	~CDxgiInfoManager();
	CDxgiInfoManager(const CDxgiInfoManager&) = delete;
	CDxgiInfoManager& operator=(const CDxgiInfoManager&) = delete;
	void Set() noexcept;
	std::vector<std::string> GetMessages() const;
private:
	unsigned long long m_next = 0u;
	struct IDXGIInfoQueue* m_pDxgiInfoQueue = nullptr;
};