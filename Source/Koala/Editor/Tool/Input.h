#ifndef KOALA__EDITOR__TOOL__INPUT
#define KOALA__EDITOR__TOOL__INPUT

#include <Koala/Editor/Service/MessageMember.h>
#include <Koala/Editor/Tool/Window.h>
#include <Koala/Utility/Macro.h>

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

#define KEY_TYPES(value, assignment) \
	assignment(Unknown) \
	\
	assignment(Space) \
	assignment(Apostrophe) \
	assignment(Comma) \
	assignment(Minus) \
	assignment(Period) \
	assignment(Slash) \
	\
	assignment(Key0) \
	value(Key1) \
	value(Key2) \
	value(Key3) \
	value(Key4) \
	value(Key5) \
	value(Key6) \
	value(Key7) \
	value(Key8) \
	value(Key9) \
	\
	assignment(Semicolon) \
	assignment(Equal) \
	\
	assignment(A) \
	value(B) \
	value(C) \
	value(D) \
	value(E) \
	value(F) \
	value(G) \
	value(H) \
	value(I) \
	value(J) \
	value(K) \
	value(L) \
	value(M) \
	value(N) \
	value(O) \
	value(P) \
	value(Q) \
	value(R) \
	value(S) \
	value(T) \
	value(U) \
	value(V) \
	value(W) \
	value(X) \
	value(Y) \
	value(Z) \
	\
	assignment(LeftBracket) \
	assignment(BackSlash) \
	assignment(RightBracket) \
	assignment(GraveAccent) \
	\
	assignment(Escape) \
	value(Enter) \
	value(Tab) \
	value(Backspace) \
	value(Insert) \
	value(Delete) \
	value(Right) \
	value(Left) \
	value(Down) \
	value(Up) \
	value(PageUp) \
	value(PageDown) \
	value(Home) \
	value(End) \
	\
	assignment(CapsLock) \
	value(ScrollLock) \
	value(NumLock) \
	value(PrintScreen) \
	value(Pause) \
	\
	assignment(F1) \
	value(F2) \
	value(F3) \
	value(F4) \
	value(F5) \
	value(F6) \
	value(F7) \
	value(F8) \
	value(F9) \
	value(F10) \
	value(F11) \
	value(F12) \
	value(F13) \
	value(F14) \
	value(F15) \
	value(F16) \
	value(F17) \
	value(F18) \
	value(F19) \
	value(F20) \
	value(F21) \
	value(F22) \
	value(F23) \
	value(F24) \
	value(F25) \
	\
	assignment(NumPad0) \
	value(NumPad1) \
	value(NumPad2) \
	value(NumPad3) \
	value(NumPad4) \
	value(NumPad5) \
	value(NumPad6) \
	value(NumPad7) \
	value(NumPad8) \
	value(NumPad9) \
	value(NumPadDecimal) \
	value(NumPadDivide) \
	value(NumPadMultiply) \
	value(NumPadSubtract) \
	value(NumPadAdd) \
	value(NumPadEnter) \
	value(NumPadEqual) \
	\
	assignment(LeftShift) \
	value(LeftControl) \
	value(LeftAlt) \
	value(LeftSuper) \
	value(RightShift) \
	value(RightControl) \
	value(RightAlt) \
	value(RightSuper) \
	value(Menu)

#define ASSIGNMENT_Unknown -1
#define ASSIGNMENT_Space ' '
#define ASSIGNMENT_Apostrophe '\''
#define ASSIGNMENT_Comma ','
#define ASSIGNMENT_Minus '-'
#define ASSIGNMENT_Period '.'
#define ASSIGNMENT_Slash '/'
#define ASSIGNMENT_Key0 '0'
#define ASSIGNMENT_Semicolon ';'
#define ASSIGNMENT_Equal '='
#define ASSIGNMENT_A 'A'
#define ASSIGNMENT_LeftBracket '['
#define ASSIGNMENT_BackSlash '\\'
#define ASSIGNMENT_RightBracket ']'
#define ASSIGNMENT_GraveAccent '`'
#define ASSIGNMENT_Escape 256
#define ASSIGNMENT_CapsLock 280
#define ASSIGNMENT_F1 290
#define ASSIGNMENT_NumPad0 320
#define ASSIGNMENT_LeftShift 340

DECLARE_ENUM(KeyType, KEY_TYPES)

} // namespace Koala::Editor::Tool

#endif // KOALA__EDITOR__TOOL__INPUT
