//========================================================================
// WindowsMessageMap.h
// https://wiki.winehq.org/List_Of_Windows_Messages
// 上 ウィンドウズメッセージ一覧
//
// 更新日：2020/07/28
// 栗城 達也
//========================================================================
#pragma once
#include <unordered_map>
#include <Windows.h>

class CWindowsMessageMap
{
public:
	CWindowsMessageMap();
	std::string operator()(DWORD msg, LPARAM lp, WPARAM wp) const;
private:
	std::unordered_map<DWORD, std::string> map;
};