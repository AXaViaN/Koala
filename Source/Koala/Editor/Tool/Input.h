#ifndef KOALA__EDITOR__TOOL__INPUT
#define KOALA__EDITOR__TOOL__INPUT

#include <Koala/Editor/Service/MessageMember.h>
#include <Koala/Editor/Tool/Window.h>

namespace Koala::Editor::Tool {

enum class KeyType;

class Input final : private Service::MessageMember
{
public:
	static void Update();

	static void RegisterKeyInput(const Window& window, Service::InputMessageType message, KeyType key);

	virtual void OnMessage(Service::MessageType type, void* data) override
	{ }
	virtual void OnInput(Service::InputMessageType type, const Service::InputMessageData& data) override
	{ }

private:
	Input()
	{ }
	~Input()
	{ }
	Input(const Input& other) = delete;
	Input(Input&& other) = delete;
	Input& operator=(const Input& other) = delete;
	Input& operator=(Input&& other) = delete;

};

enum class KeyType
{
	Unknown				= -1,

	Space				= ' ',
	Apostrophe			= '\'',
	Comma				= ',',
	Minus				= '-',
	Period				= '.',
	Slash				= '/',

	Key0				= '0',
	Key1,
	Key2,
	Key3,
	Key4,
	Key5,
	Key6,
	Key7,
	Key8,
	Key9,

	Semicolon			= ';',
	Equal				= '=',

	A					= 'A',
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,

	LeftBracket			= '[',
	BackSlash			= '\\',
	RightBracket		= ']',
	GraveAccent			= '`',

	Escape				= 256,
	Enter,
	Tab,
	Backspace,
	Insert,
	Delete,
	Right,
	Left,
	Down,
	Up,
	PageUp,
	PageDown,
	Home,
	End,

	CapsLock			= 280,
	ScrollLock,
	NumLock,
	PrintScreen,
	Pause,

	F1					= 290,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	F13,
	F14,
	F15,
	F16,
	F17,
	F18,
	F19,
	F20,
	F21,
	F22,
	F23,
	F24,
	F25,

	NumPad0				= 320,
	NumPad1,
	NumPad2,
	NumPad3,
	NumPad4,
	NumPad5,
	NumPad6,
	NumPad7,
	NumPad8,
	NumPad9,
	NumPadDecimal,
	NumPadDivide,
	NumPadMultiply,
	NumPadSubtract,
	NumPadAdd,
	NumPadEnter,
	NumPadEqual,

	LeftShift			= 340,
	LeftControl,
	LeftAlt,
	LeftSuper,
	RightShift,
	RightControl,
	RightAlt,
	RightSuper,
	Menu
};

} // namespace Koala::Editor::Tool

#endif // KOALA__EDITOR__TOOL__INPUT
