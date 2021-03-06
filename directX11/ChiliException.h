//========================================================================
// ChiliException.cpp
// 例外処理 他のクラスへ派生する
//
// 更新日：2020/08/02
// 栗城 達也
//========================================================================
#pragma once
#include <exception>
#include <string>

class CChiliException : public std::exception
{
public:
	CChiliException(int line, const char* file) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;
private:
	int m_line;
	std::string m_file;
protected:
	mutable std::string m_whatBuffer;
};