//========================================================================
// ChiliTimer.h
// タイマー
//
// 更新日：2020/08/02
// 栗城 達也
//========================================================================
#pragma once
#include <chrono>

class CTimer
{
public:
	CTimer();
	float Mark();
	float Peek() const;
private:
	std::chrono::steady_clock::time_point last;
};