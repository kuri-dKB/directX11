//========================================================================
// GDIPlusManger.h
// 
//
// �X�V���F2020/08/10
// �I�� �B��
//========================================================================
#pragma once
#include "ChiliWin.h"

class CGDIPlusManager
{
public:
	CGDIPlusManager();
	~CGDIPlusManager();
private:
	static ULONG_PTR m_token;
	static int m_refCount;
};