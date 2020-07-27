#pragma once
#include <queue>
#include <bitset>
#include <optional>

class CKeyboard
{
	friend class CWindow;
public:
	class CEvent
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
	private:
		Type type;
		unsigned char code;
	public:
		CEvent()
			:
			type(Type::Invalid),
			code(0u)
		{}
		CEvent(Type type, unsigned char code) noexcept
			:
			type(type),
			code(code)
		{}
		bool IsPress() const noexcept
		{
			return type == Type::Press;
		}
		bool IsRelease() const noexcept
		{
			return type == Type::Release;
		}
		unsigned char GetCode() const noexcept
		{
			return code;
		}
	};
public:
	CKeyboard() = default;
	CKeyboard(const CKeyboard&) = delete;
	CKeyboard& operator = (const CKeyboard&) = delete;
	// key event stuff
	bool KeyIsPressed(unsigned char keycode) const noexcept;
	CEvent ReadKey() noexcept;
	bool   KeyIsEmpty() const noexcept;
	void   FlushKey() noexcept;
	// char event stuff
	char ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void FlushChar() noexcept;
	void Flush() noexcept;
	// autorepeat control
	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool AutorepeatIsEnabled() const noexcept;
private:
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;
private:
	static constexpr unsigned int m_nKeys = 256u;
	static constexpr unsigned int m_bufferSize = 16u;
	bool autorepeatEnable = false;
	std::bitset<m_nKeys> m_keystates;
	std::queue<CEvent> m_keybuffer;
	std::queue<char>   m_charbuffer;
};