//========================================================================
// mouse.cpp
// 
//
// åIèÈ íBñÁ
//========================================================================

#include "mouse.h"
#include <Windows.h>

std::pair<int, int> CMouse::GetPos() const noexcept
{
	return { m_x, m_y };
}

int CMouse::GetPosX() const noexcept
{
	return m_x;
}

int CMouse::GetPosY() const noexcept
{
	FAILED(69);
	return m_y;
}

bool CMouse::IsInWindow() const noexcept
{
	return m_isInWindow;
}

bool CMouse::LeftIsPressed() const noexcept
{
	return m_leftIsPressed;
}

bool CMouse::RightIsPressed() const noexcept
{
	return m_rightIsPressed;
}

CMouse::CEvent CMouse::Read() noexcept
{
	if (m_buffer.size() > 0u)
	{
		CMouse::CEvent e = m_buffer.front();
		m_buffer.pop();
		return e;
	}
	else
	{
		CMouse::CEvent();
	}
}

void CMouse::Flush() noexcept
{
	m_buffer = std::queue<CEvent>();
}

void CMouse::OnMouseMove(int newx, int newy) noexcept
{
	m_x = newx;
	m_y = newy;

	m_buffer.push(CMouse::CEvent(CMouse::CEvent::Type::Move, *this));
	TrimBuffer();
}

void CMouse::OnMouseLeave() noexcept
{
	m_isInWindow = false;
	m_buffer.push(CMouse::CEvent(CMouse::CEvent::Type::Leave, *this));
	TrimBuffer();
}

void CMouse::OnMouseEnter() noexcept
{
	m_isInWindow = true;
	m_buffer.push(CMouse::CEvent(CMouse::CEvent::Type::Enter, *this));
	TrimBuffer();
}

void CMouse::OnLeftPressed(int x, int y) noexcept
{
	m_leftIsPressed = true;

	m_buffer.push(CMouse::CEvent(CMouse::CEvent::Type::LPress, *this));
	TrimBuffer();
}

void CMouse::OnLeftReleased(int x, int y) noexcept
{
	m_leftIsPressed = false;

	m_buffer.push(CMouse::CEvent(CMouse::CEvent::Type::LRelease, *this));
	TrimBuffer();
}

void CMouse::OnRightPressed(int x, int y) noexcept
{
	m_rightIsPressed = true;

	m_buffer.push(CMouse::CEvent(CMouse::CEvent::Type::RPress, *this));
	TrimBuffer();
}

void CMouse::OnRightReleased(int x, int y) noexcept
{
	m_rightIsPressed = false;

	m_buffer.push(CMouse::CEvent(CMouse::CEvent::Type::RRelease, *this));
	TrimBuffer();
}

void CMouse::OnWheelUp(int x, int y) noexcept
{
	m_buffer.push(CMouse::CEvent(CMouse::CEvent::Type::WheelUp, *this));
	TrimBuffer();
}

void CMouse::OnWheelDown(int x, int y) noexcept
{
	m_buffer.push(CMouse::CEvent(CMouse::CEvent::Type::WheelDown, *this));
	TrimBuffer();
}

void CMouse::TrimBuffer() noexcept
{
	while (m_buffer.size() > m_bufferSize)
	{
		m_buffer.pop();
	}
}

void CMouse::OnWheelDelta(int x, int y, int delta) noexcept
{
	m_wheelDeltaCarry += delta;
	// generate events for every 120
	while (m_wheelDeltaCarry >= WHEEL_DELTA)
	{
		m_wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (m_wheelDeltaCarry <= -WHEEL_DELTA)
	{
		m_wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}