#include <Koala/Utility/Core/FunctionManager.h>
#include <Koala/Utility/IDGenerator.h>

namespace Koala::Utility::Core {

static constexpr size_t UserFunctionIDStart = 0x80000000;

static std::vector<FunctionInfo>& GetFunctionTemplates()
{
	static std::vector<FunctionInfo> functionTemplates;
	return functionTemplates;
}
static IDGenerator& GetIDGenerator()
{
	static IDGenerator idGenerator;
	return idGenerator;
}

static constexpr size_t IndexNotFound = -1;
static size_t GetTemplateIndex(FunctionID id);

static char SetupDefaultFunctions();
static char g_DummyInitializer = SetupDefaultFunctions();

FunctionID FunctionManager::Add(const FunctionInfo& functionInfo)
{
	GetFunctionTemplates().emplace_back(functionInfo);
	
	auto& function = GetFunctionTemplates().back();
	function.ID = GetIDGenerator().GetNextID();
	if(function.NameText != Text::Empty)
	{
		function.Name = Resource::GetText(function.NameText);
	}

	return function.ID;
}
bool FunctionManager::Remove(FunctionID id)
{
	auto index = GetTemplateIndex(id);
	if(index != IndexNotFound)
	{
		GetFunctionTemplates().erase(GetFunctionTemplates().begin() + index);
		return true;
	}
	return false;
}
void FunctionManager::RemoveAllUserFunctions()
{
	auto lastID = GetIDGenerator().GetNextID()-1;
	while(lastID >= UserFunctionIDStart)
	{
		Remove(lastID--);
	}

	GetIDGenerator().SetNextID(UserFunctionIDStart);
}
bool FunctionManager::Edit(FunctionID id, const FunctionInfo& functionInfo)
{
	auto index = GetTemplateIndex(id);
	if(index != IndexNotFound)
	{
		GetFunctionTemplates()[index] = functionInfo;
		return true;
	}
	return false;
}
const FunctionInfo& FunctionManager::Get(FunctionID id)
{
	auto index = GetTemplateIndex(id);
	if(index != IndexNotFound)
	{
		return GetFunctionTemplates()[index];
	}
	else
	{
		static FunctionInfo dummy;
		return dummy;
	}
}
const FunctionInfo& FunctionManager::GetDefault(Text name)
{
	for( auto& functionTemplate : GetFunctionTemplates() )
	{
		if(functionTemplate.NameText == name)
		{
			return functionTemplate;
		}
	}

	static FunctionInfo dummy;
	return dummy;
}

std::vector<FunctionInfo> FunctionManager::GetUserFunctions()
{
	auto nextID = GetIDGenerator().GetNextID();
	GetIDGenerator().SetNextID(nextID);

	std::vector<FunctionInfo> userFunctions;
	for( size_t i=UserFunctionIDStart ; i<nextID ; ++i )
	{
		auto& function = Get(i);
		if(function.ID != 0)
		{
			userFunctions.emplace_back(function);
		}
	}

	return userFunctions;
}
FunctionID FunctionManager::PeekNextID()
{
	FunctionID nextID = GetIDGenerator().GetNextID();
	GetIDGenerator().SetNextID(nextID);
	return nextID;
}
void FunctionManager::SetNextID(FunctionID id)
{
	GetIDGenerator().SetNextID(id);
}

static size_t GetTemplateIndex(FunctionID id)
{
	// Start from estimated position
	size_t i = id - 1;
	if(id > GetFunctionTemplates().size())
	{
		i = GetFunctionTemplates().size() - 1u;
	}

	while((int)i >= 0)
	{
		if(GetFunctionTemplates()[i].ID == id)
		{
			break;
		}
		else if(GetFunctionTemplates()[i].ID < id)
		{
			return IndexNotFound;
		}

		--i;
	}

	return i;
}

static char SetupDefaultFunctions()
{
	// Program node
	{
		FunctionInfo functionInfo;
		functionInfo.NameText = Text::Program;
		functionInfo.FrontSlots.emplace_back("", VariableType::None);

		FunctionManager::Add(functionInfo);
	}

	// Add, subtract, multiply, divide, power, modulo
	{
		FunctionInfo functionInfo;
		functionInfo.NameText = Text::Add;
		functionInfo.BackSlots.emplace_back("", VariableType::None);
		functionInfo.BackSlots.emplace_back("X", VariableType::Float64);
		functionInfo.BackSlots.emplace_back("Y", VariableType::Float64);
		functionInfo.FrontSlots.emplace_back("", VariableType::None);
		functionInfo.FrontSlots.emplace_back(Text::Result, VariableType::Float64);

		FunctionManager::Add(functionInfo);

		functionInfo.NameText = Text::Subtract;
		FunctionManager::Add(functionInfo);

		functionInfo.NameText = Text::Multiply;
		FunctionManager::Add(functionInfo);

		functionInfo.NameText = Text::Divide;
		FunctionManager::Add(functionInfo);

		functionInfo.NameText = Text::Power;
		FunctionManager::Add(functionInfo);

		functionInfo.NameText = Text::Modulo;
		FunctionManager::Add(functionInfo);
	}

	// SquareRoot
	{
		FunctionInfo functionInfo;
		functionInfo.NameText = Text::SquareRoot;
		functionInfo.BackSlots.emplace_back("", VariableType::None);
		functionInfo.BackSlots.emplace_back("X", VariableType::Float64);
		functionInfo.FrontSlots.emplace_back("", VariableType::None);
		functionInfo.FrontSlots.emplace_back(Text::Result, VariableType::Float64);

		FunctionManager::Add(functionInfo);
	}

	// Print
	{
		FunctionInfo functionInfo;
		functionInfo.NameText = Text::Print;
		functionInfo.BackSlots.emplace_back("", VariableType::None);
		functionInfo.BackSlots.emplace_back("", VariableType::String);
		functionInfo.FrontSlots.emplace_back("", VariableType::None);

		FunctionManager::Add(functionInfo);
	}

	// ReadNumber
	{
		FunctionInfo functionInfo;
		functionInfo.NameText = Text::ReadNumber;
		functionInfo.BackSlots.emplace_back("", VariableType::None);
		functionInfo.FrontSlots.emplace_back("", VariableType::None);
		functionInfo.FrontSlots.emplace_back("", VariableType::Float64);

		FunctionManager::Add(functionInfo);
	}

	// ReadString
	{
		FunctionInfo functionInfo;
		functionInfo.NameText = Text::ReadString;
		functionInfo.BackSlots.emplace_back("", VariableType::None);
		functionInfo.FrontSlots.emplace_back("", VariableType::None);
		functionInfo.FrontSlots.emplace_back("", VariableType::String);

		FunctionManager::Add(functionInfo);
	}

	// If
	{
		FunctionInfo functionInfo;
		functionInfo.NameText = Text::If;
		functionInfo.BackSlots.emplace_back("", VariableType::None);
		functionInfo.BackSlots.emplace_back(Text::Condition, VariableType::Boolean);
		functionInfo.FrontSlots.emplace_back(Text::IfTrue, VariableType::None);
		functionInfo.FrontSlots.emplace_back(Text::IfFalse, VariableType::None);

		FunctionManager::Add(functionInfo);
	}

	// ForLoop
	{
		FunctionInfo functionInfo;
		functionInfo.NameText = Text::ForLoop;
		functionInfo.BackSlots.emplace_back("", VariableType::None);
		functionInfo.BackSlots.emplace_back(Text::Start, VariableType::Float64);
		functionInfo.BackSlots.emplace_back(Text::End, VariableType::Float64);
		functionInfo.FrontSlots.emplace_back("", VariableType::None);
		functionInfo.FrontSlots.emplace_back(Text::Index, VariableType::Float64);
		functionInfo.FrontSlots.emplace_back(Text::Loop, VariableType::None);

		FunctionManager::Add(functionInfo);
	}

	// WhileLoop
	{
		FunctionInfo functionInfo;
		functionInfo.NameText = Text::WhileLoop;
		functionInfo.BackSlots.emplace_back("", VariableType::None);
		functionInfo.BackSlots.emplace_back(Text::Condition, VariableType::Boolean);
		functionInfo.FrontSlots.emplace_back("", VariableType::None);
		functionInfo.FrontSlots.emplace_back(Text::Loop, VariableType::None);

		FunctionManager::Add(functionInfo);
	}

	// Greater, Smaller, NumberEquals
	{
		FunctionInfo functionInfo;
		functionInfo.NameText = Text::Greater;
		functionInfo.BackSlots.emplace_back("", VariableType::None);
		functionInfo.BackSlots.emplace_back("X", VariableType::Float64);
		functionInfo.BackSlots.emplace_back("Y", VariableType::Float64);
		functionInfo.FrontSlots.emplace_back("", VariableType::None);
		functionInfo.FrontSlots.emplace_back("", VariableType::Boolean);

		FunctionManager::Add(functionInfo);

		functionInfo.NameText = Text::Smaller;
		FunctionManager::Add(functionInfo);

		functionInfo.NameText = Text::NumberEquals;
		FunctionManager::Add(functionInfo);
	}

	// And, Or
	{
		FunctionInfo functionInfo;
		functionInfo.NameText = Text::And;
		functionInfo.BackSlots.emplace_back("", VariableType::None);
		functionInfo.BackSlots.emplace_back("X", VariableType::Boolean);
		functionInfo.BackSlots.emplace_back("Y", VariableType::Boolean);
		functionInfo.FrontSlots.emplace_back("", VariableType::None);
		functionInfo.FrontSlots.emplace_back("", VariableType::Boolean);

		FunctionManager::Add(functionInfo);

		functionInfo.NameText = Text::Or;
		FunctionManager::Add(functionInfo);
	}

	// StringEquals
	{
		FunctionInfo functionInfo;
		functionInfo.NameText = Text::StringEquals;
		functionInfo.BackSlots.emplace_back("", VariableType::None);
		functionInfo.BackSlots.emplace_back("X", VariableType::String);
		functionInfo.BackSlots.emplace_back("Y", VariableType::String);
		functionInfo.FrontSlots.emplace_back("", VariableType::None);
		functionInfo.FrontSlots.emplace_back("", VariableType::Boolean);

		FunctionManager::Add(functionInfo);
	}

	// NumberToString
	{
		FunctionInfo functionInfo;
		functionInfo.NameText = Text::NumberToString;
		functionInfo.BackSlots.emplace_back("", VariableType::None);
		functionInfo.BackSlots.emplace_back("", VariableType::Float64);
		functionInfo.FrontSlots.emplace_back("", VariableType::None);
		functionInfo.FrontSlots.emplace_back("", VariableType::String);

		FunctionManager::Add(functionInfo);
	}

	// StringToNumber
	{
		FunctionInfo functionInfo;
		functionInfo.NameText = Text::StringToNumber;
		functionInfo.BackSlots.emplace_back("", VariableType::None);
		functionInfo.BackSlots.emplace_back("", VariableType::String);
		functionInfo.FrontSlots.emplace_back("", VariableType::None);
		functionInfo.FrontSlots.emplace_back("", VariableType::Float64);

		FunctionManager::Add(functionInfo);
	}

	// First half is reserved for default functions
	GetIDGenerator().SetNextID(UserFunctionIDStart);
	return '\0';
}

} // namespace Koala::Utility::Core
