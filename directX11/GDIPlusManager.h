//========================================================================
// GDIPlusManger.h
// 
//
// 更新日：2020/08/10
// 栗城 達也
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