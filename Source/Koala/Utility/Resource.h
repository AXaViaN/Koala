#ifndef KOALA__UTILITY__RESOURCE
#define KOALA__UTILITY__RESOURCE

#include <string>
#include <map>

namespace Koala::Utility {

enum class Text : unsigned int;
enum class Icon;

class Resource
{
public:
	static void Initialize(bool loadIcons=false);

	static const std::string& GetText(Text text);
	static void* GetIcon(Icon icon);

	static const std::string& LogFilePath()
	{
		static const std::string path = "Data/user.log";
		return path;
	}

private:
	Resource();
	~Resource() noexcept;
	Resource(const Resource& other) = delete;
	Resource(Resource&& other) = delete;
	Resource& operator=(const Resource& other) = delete;
	Resource& operator=(Resource&& other) = delete;

	static Resource& Instance()
	{
		static Resource instance;
		return instance;
	}

private:
	std::map<Text, std::string> m_TextMap;
	std::map<Icon, void*> m_IconMap;

};

enum class Text : unsigned int
{
	Empty,

	ControlPanel,
	Details,
	Palette,
	Program,

	Functions,
	Variables,

	Arithmetic,
	Add,
	Subtract,
	Multiply,
	Divide,
	SquareRoot,
	Power,
	Modulo,

	System,
	Print,
	ReadNumber,
	ReadString,

	Flow,
	If,
	ForLoop,
	WhileLoop,
	Loop,
	Condition,
	True,
	False,
	IfTrue,
	IfFalse,

	General,
	Greater,
	Smaller,
	And,
	Or,
	NumberEquals,
	StringEquals,
	NumberToString,
	StringToNumber,

	Search,
	Result,
	Value,
	Start,
	End,
	Index,

	KoalaEditor,
	NewProject,
	Save,
	Open,
	Cancel,
	FileName,

	VM_DataSizeMismatchError = std::numeric_limits<unsigned int>::max()/2u + 1,
	VM_KoaFile,
	VM_CodeExecutionError,

	VM_CouldNotBeFound,
	VM_PressEnterToExit,

	VM_DivideByZeroError,
	VM_NumberRequiredError,
	VM_SetLocalModeError,
	VM_GetLocalModeError,
	VM_DerefModeError,
	VM_AssignrefModeError,
	VM_GetConstModeError,
	VM_AllocError,
	VM_InvalidBytecodeError,
	VM_StackError,
	VM_StackFrameError,

	VM_Push8StackOverflow,
	VM_Push64iStackOverflow,
	VM_Push64fStackOverflow,
	VM_Pop8OnEmptyStack,
	VM_Pop64iOnEmptyStack,
	VM_Pop64fOnEmptyStack,
	VM_Top8OnEmptyStack,
	VM_Top64iOnEmptyStack,
	VM_Top64fOnEmptyStack,

	VM_PushFrameStackOverflow,
	VM_ReserveStackOverflow,
	VM_PopFrameOnFirstFrame,
	VM_GetVariableRef8OutOfBound,
	VM_GetVariableRef64iOutOfBound,
	VM_GetVariableRef64fOutOfBound,
	VM_GetConstant8OutOfBound,
	VM_GetConstant64iOutOfBound,
	VM_GetConstant64fOutOfBound,
};

enum class Icon
{
	Empty,

	New,
	Open,
	Save,
	Settings,
	Compile,
	Run,

	NewFunction,
	NewVariable,
};

} // namespace Koala::Utility

#endif // KOALA__UTILITY__RESOURCE
