#ifndef KOALA__EDITOR__SERVICE__MESSAGE_TYPE
#define KOALA__EDITOR__SERVICE__MESSAGE_TYPE

#include <Koala/Utility/Macro.h>
#include <string>

namespace Koala::Editor::Tool {
class Window;
enum class KeyType;
}
namespace Koala::Utility {
enum class LogType;
}

namespace Koala::Editor::Service {

// Input messages

#define INPUT_TYPES(value, assignment) \
	value(KeyPress) \
	value(KeyRelease) \
	value(KeyHold)

DECLARE_ENUM(InputMessageType, INPUT_TYPES)

struct InputMessageData
{
	const Tool::Window& Window;
	const Tool::KeyType Key = static_cast<Tool::KeyType>(-1);
};

// Generic messages

#define MESSAGE_TYPES(value, assignment) \
	value(ProgramExit) \
	value(LogInfo) \
	value(LogError)

DECLARE_ENUM(MessageType, MESSAGE_TYPES)

struct LogMessageData
{
	std::string Message;
	std::string Function;
	size_t Line = 0;
};
#define GenerateLogMessageData() \
	Koala::Editor::Service::LogMessageData {"", __FUNCTION__, size_t(__LINE__)}

} // namespace Koala::Editor::Service

#endif // KOALA__EDITOR__SERVICE__MESSAGE_TYPE
