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

	// Add
	{
		FunctionInfo functionInfo;
		functionInfo.NameText = Text::Add;
		functionInfo.BackSlots.emplace_back("", Utility::Core::VariableType::None);
		functionInfo.BackSlots.emplace_back("X", Utility::Core::VariableType::Float64);
		functionInfo.BackSlots.emplace_back("Y", Utility::Core::VariableType::Float64);
		functionInfo.FrontSlots.emplace_back("", Utility::Core::VariableType::None);
		functionInfo.FrontSlots.emplace_back(Text::Result, Utility::Core::VariableType::Float64);

		FunctionManager::Add(functionInfo);
	}

	// TODO: Add all the default functions from Palette

	// First half is reserved for default functions
	GetIDGenerator().SetNextID(static_cast<size_t>(0x80000000));
	return '\0';
}

} // namespace Koala::Utility::Core
