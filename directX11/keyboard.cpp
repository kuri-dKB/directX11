//========================================================================
// Keyboard.cpp
//
// 更新日：2020/07/30
// 栗城 達也
//========================================================================
#include "keyboard.h"

bool CKeyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
	return m_keystates[keycode];
}

CKeyboard::CEvent CKeyboard::ReadKey() noexcept
{
	if (m_keybuffer.size() > 0u)
	{
		CKeyboard::CEvent e = m_keybuffer.front();
		m_keybuffer.pop();
		return e;
	}
	else
	{
		return CKeyboard::CEvent();
	}
}

bool CKeyboard::KeyIsEmpty() const noexcept
{
	return m_keybuffer.empty();
}

char CKeyboard::ReadChar() noexcept
{
	if (m_charbuffer.size() > 0u)
	{
		unsigned char charcode = m_charbuffer.front();
		m_charbuffer.pop();
		return charcode;
	}
	else
	{
		return 0;
	}
}

bool CKeyboard::CharIsEmpty() const noexcept
{
	return m_charbuffer.empty();
}

void CKeyboard::FlushKey() noexcept
{
	m_keybuffer = std::queue<CEvent>();
}

void CKeyboard::FlushChar() noexcept
{
	m_charbuffer = std::queue<char>();
}

void CKeyboard::Flush() noexcept
{
	FlushKey();
	FlushChar();
}

void CKeyboard::EnableAutorepeat() noexcept
{
	autorepeatEnable = true;
}

void CKeyboard::DisableAutorepeat() noexcept
{
	autorepeatEnable = false;
}

bool CKeyboard::AutorepeatIsEnabled() const noexcept
{
	return autorepeatEnable;
}

void CKeyboard::OnKeyPressed(unsigned char keycode) noexcept
{
	m_keystates[keycode] = true;
	m_keybuffer.push(CKeyboard::CEvent(CKeyboard::CEvent::Type::Press, keycode));
	TrimBuffer(m_keybuffer);
}

void CKeyboard::OnKeyReleased(unsigned char keycode) noexcept
{
	m_keystates[keycode] = false;
	m_keybuffer.push(CKeyboard::CEvent(CKeyboard::CEvent::Type::Release, keycode));
	TrimBuffer(m_keybuffer);
}

void CKeyboard::OnChar(char character) noexcept
{
	m_charbuffer.push(character);
	TrimBuffer(m_charbuffer);
}

void CKeyboard::ClearState() noexcept
{
	m_keystates.reset();
}

template<typename T>
void CKeyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size() > m_bufferSize)
	{
		buffer.pop();
	}
}