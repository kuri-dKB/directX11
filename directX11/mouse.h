//========================================================================
// mouse.h
// マウス操作
//
// 更新日：2020/07/28
// 栗城 達也
//========================================================================
#pragma once
#include <queue>

class CMouse
{
	friend class CWindow;
public:
	class  CEvent
	{
	public:
		enum class Type
		{// イベントタイプ
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid 
		};
	private:
		Type type;
		bool leftIsPressed;  // クリックの判定
		bool rightIsPressed; //
		int x; // 座標
		int y;
	public:
		CEvent() noexcept
			: // 初期値
			type(Type::Invalid),
			leftIsPressed(false),
			rightIsPressed(false),
			x(0),
			y(0)
		{}
		CEvent(Type type, const CMouse& parent) noexcept
			: // 初期値
			type(type),
			leftIsPressed(parent.m_leftIsPressed),
			rightIsPressed(parent.m_rightIsPressed),
			x(parent.m_x),
			y(parent.m_y)
		{}
		bool IsValid() const noexcept
		{
			return type != Type::Invalid;
		}
		Type GetType() const noexcept
		{
			return type;
		}
		std::pair<int,int> GetPos() const noexcept
		{
			return { x, y };
		}
		int GetPosX() const noexcept
		{
			return x;
		}
		int GetPosY() const noexcept
		{
			return y;
		}
		bool LeftIsPressed() const noexcept
		{
			return leftIsPressed;
		}
		bool RinghtIsPressed() const noexcept
		{
			return rightIsPressed;
		}
	};
public:
	CMouse() = default;
	CMouse(const CMouse&) = delete;            // 禁止
	CMouse& operator=(const CMouse&) = delete; // 禁止
	std::pair<int, int> GetPos() const noexcept; // std::pair ２つの要素を入れられる (x,y)
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool IsInWindow() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	CMouse::CEvent Read() noexcept;
	bool IsEmpty() const noexcept
	{
		return m_buffer.empty();
	}
	void Flush() noexcept; // 初期、もとに戻す
private:
	// マウスの状態
	void OnMouseMove(int x, int y) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer() noexcept; // バッファのトリミング
	void OnWheelDelta(int x, int y, int delta) noexcept;
private:
	static constexpr unsigned int m_bufferSize = 16u; // constexpr -> https://docs.microsoft.com/ja-jp/cpp/cpp/constexpr-cpp?view=vs-2019
	int  m_x;
	int  m_y;
	bool m_leftIsPressed = false;
	bool m_rightIsPressed = false;
	bool m_isInWindow = false;
	int  m_wheelDeltaCarry = 0;
	std::queue<CEvent> m_buffer;
};