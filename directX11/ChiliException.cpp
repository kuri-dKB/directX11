//========================================================================
// ChiliException.cpp
// 例外処理
//
// 更新日：2020/08/02
// 栗城 達也
//========================================================================
#include "ChiliException.h"
#include <sstream>

CChiliException::CChiliException(int line, const char* file) noexcept
	:
	m_line(line),
	m_file(file)
{}

const char* CChiliException::what() const  noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	m_whatBuffer = oss.str();
	return m_whatBuffer.c_str();
}

const char* CChiliException::GetType() const noexcept
{
	return "Chili Exception";
}

int CChiliException::GetLine() const noexcept
{
	return m_line;
}

const std::string& CChiliException::GetFile() const noexcept
{
	return m_file;
}

std::string CChiliException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << m_file << std::endl
		<< "[Line] " << m_line;
	return oss.str();
}

