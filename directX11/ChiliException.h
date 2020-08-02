//========================================================================
// ChiliException.cpp
// ��O���� ���̃N���X�֔h������
//
// �X�V���F2020/08/02
// �I�� �B��
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