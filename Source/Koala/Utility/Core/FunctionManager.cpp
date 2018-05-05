#include <Koala/Utility/Core/FunctionManager.h>
#include <Koala/Utility/IDGenerator.h>

namespace Koala::Utility::Core {

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

static constexpr int IndexNotFound = -1;
static int GetTemplateIndex(FunctionID id);

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

static int GetTemplateIndex(FunctionID id)
{
	// Start from estimated position
	int i = id - 1;
	if(id > GetFunctionTemplates().size())
	{
		i = GetFunctionTemplates().size()-1;
	}

	while(i >= 0)
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
		functionInfo.BackSlots.emplace_back(Text::Value, VariableType::Float64);
		functionInfo.FrontSlots.emplace_back(Text::IfTrue, VariableType::None);
		functionInfo.FrontSlots.emplace_back(Text::IfFalse, VariableType::None);

		FunctionManager::Add(functionInfo);
	}

	// Loop
	{
		FunctionInfo functionInfo;
		functionInfo.NameText = Text::Loop;
		functionInfo.BackSlots.emplace_back("", VariableType::None);
		functionInfo.BackSlots.emplace_back(Text::Start, VariableType::Float64);
		functionInfo.BackSlots.emplace_back(Text::End, VariableType::Float64);
		functionInfo.FrontSlots.emplace_back("", VariableType::None);
		functionInfo.FrontSlots.emplace_back(Text::Index, VariableType::Float64);
		functionInfo.FrontSlots.emplace_back(Text::Loop, VariableType::None);

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
	GetIDGenerator().SetNextID(static_cast<size_t>(0x80000000));
	return '\0';
}

} // namespace Koala::Utility::Core
