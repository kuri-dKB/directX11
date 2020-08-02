//========================================================================
// WindowsMessageMap.h
// https://wiki.winehq.org/List_Of_Windows_Messages
// �� �E�B���h�E�Y���b�Z�[�W�ꗗ
//
// �X�V���F2020/07/28
// �I�� �B��
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