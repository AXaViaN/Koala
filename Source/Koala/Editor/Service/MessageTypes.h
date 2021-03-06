#ifndef KOALA__EDITOR__SERVICE__MESSAGE_TYPE
#define KOALA__EDITOR__SERVICE__MESSAGE_TYPE

#include <Koala/Editor/Gfx/Vector2.h>
#include <Koala/Utility/Macro.h>
#include <string>
#include <vector>

namespace Koala::Editor {
struct Function;
}
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
enum class SlotSide;
using NodeID = size_t;
using FunctionID = size_t;
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
	value(NewProject) \
	value(SaveProject) \
	value(LoadProject) \
	\
	value(LogInfo) \
	value(LogError) \
	\
	value(NodeMove) \
	value(NodeRemove) \
	value(ConnectionBegin) \
	value(ConnectionEnd) \
	value(ConnectionCancel) \
	value(ConnectionRemove) \
	\
	value(RequestNode) \
	value(SpawnNode) \
	\
	value(EditUserFunction) \
	value(RemoveUserFunction) \
	value(EditVariableFunction) \
	value(RemoveVariableFunction) \
	\
	value(FunctionSelected) \

DECLARE_ENUM(MessageType, MESSAGE_TYPES)

struct SaveProjectData
{
	std::string ProjectName;

	std::vector<Function> Functions;
	size_t SelectedFunction;

	std::vector<Utility::Core::FunctionID> UserFunctionIDs;
	std::vector<std::vector<Utility::Core::FunctionID>> VariableFunctionIDs;
};
using LoadProjectData = SaveProjectData;

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
struct ConnectionBeginData
{
	Utility::Core::Node& Node;
	const Utility::Core::SlotSide SlotSide;
	const size_t SlotIndex;
};
using ConnectionEndData = ConnectionBeginData;
using ConnectionRemoveData = ConnectionBeginData;

struct RequestNodeData
{
	const Utility::Core::NodeID NodeID = 0;
	const Utility::Core::Node* Node;
	Gfx::Vector2 NodePosition;
};
struct SpawnNodeData
{
	const Utility::Core::FunctionID FunctionID;
};

struct EditUserFunctionData
{
	const Utility::Core::FunctionID FunctionID;
};
using RemoveUserFunctionData = EditUserFunctionData;
using EditVariableFunctionData = EditUserFunctionData;
using RemoveVariableFunctionData = EditUserFunctionData;

struct FunctionSelectedData
{
	const size_t FunctionIndex;
};

} // namespace Koala::Editor::Service

#endif // KOALA__EDITOR__SERVICE__MESSAGE_TYPE
