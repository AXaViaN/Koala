#ifndef KOALA__EDITOR__SERVICE__MESSAGE_TYPE
#define KOALA__EDITOR__SERVICE__MESSAGE_TYPE

namespace Koala::Editor::Tool {
class Window;
enum class KeyType;
}

namespace Koala::Editor::Service {

enum class InputMessageType
{
	KeyPress,
	KeyRelease,
	KeyHold
};
struct InputMessageData
{
	const Tool::Window& Window;
	const Tool::KeyType Key = static_cast<Tool::KeyType>(-1);
};

enum class MessageType
{
	ProgramExit
};

} // namespace Koala::Editor::Service

#endif // KOALA__EDITOR__SERVICE__MESSAGE_TYPE
