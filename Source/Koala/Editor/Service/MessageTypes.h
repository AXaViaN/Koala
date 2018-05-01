#ifndef KOALA__EDITOR__SERVICE__MESSAGE_TYPE
#define KOALA__EDITOR__SERVICE__MESSAGE_TYPE

#include <Koala/Editor/Gfx/Vector2.h>
#include <Koala/Utility/Macro.h>
#include <string>

namespace Koala::Editor::Tool {
class Window;
enum class KeyType;
enum class MouseButtonType;
}
namespace Koala::Utility {
enum class LogType;
}
namespace Koala::Utility::Core {
class Node;
using NodeID = size_t;
}

namespace Koala::Editor::Service {

// Input messages

#define INPUT_TYPES(value, assignment) \
	value(KeyPress) \
	value(KeyRelease) \
	value(KeyHold) \
	value(MousePress) \
	value(MouseRelease) \
	value(MouseHold) \
	value(MouseScroll)

DECLARE_ENUM(InputMessageType, INPUT_TYPES)

struct InputMessageData
{
	const Tool::Window& Window;
	const Tool::KeyType Key = static_cast<Tool::KeyType>(-1);
	const Tool::MouseButtonType Button = static_cast<Tool::MouseButtonType>(-1);
	const Gfx::Vector2 Scroll;
};

// Generic messages

#define MESSAGE_TYPES(value, assignment) \
	value(ProgramExit) \
	value(LogInfo) \
	value(LogError) \
	\
	value(NodeMove) \
	value(NodeRemove) \
	\
	value(RequestNode)

DECLARE_ENUM(MessageType, MESSAGE_TYPES)

struct LogMessageData
{
	std::string Message;
	std::string Function;
	size_t Line = 0;
};
#define GenerateLogMessageData() \
	Koala::Editor::Service::LogMessageData {"", __FUNCTION__, size_t(__LINE__)}

struct NodeMoveData
{
	const Utility::Core::Node& Node;
	const Gfx::Vector2 RawMouseDelta;
};
struct NodeRemoveData
{
	const Utility::Core::Node& Node;
};

struct RequestNodeData
{
	const Utility::Core::NodeID NodeID = 0;
	const Utility::Core::Node* Node;
	Gfx::Vector2 NodePosition;
};

} // namespace Koala::Editor::Service

#endif // KOALA__EDITOR__SERVICE__MESSAGE_TYPE
