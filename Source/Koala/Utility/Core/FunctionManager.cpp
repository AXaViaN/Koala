#include <Koala/Utility/Core/FunctionManager.h>
#include <Koala/Utility/IDGenerator.h>

namespace Koala::Utility::Core {

static std::vector<FunctionInfo> g_FunctionTemplates;
static IDGenerator g_IDGenerator;

static constexpr int IndexNotFound = -1;
static int GetTemplateIndex(FunctionID id);

FunctionID FunctionManager::Add(const FunctionInfo& functionInfo)
{
	g_FunctionTemplates.emplace_back(functionInfo);

	auto& function = g_FunctionTemplates.back();
	function.ID = g_IDGenerator.GetNextID();
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
		g_FunctionTemplates.erase(g_FunctionTemplates.begin() + index);
		return true;
	}
	return false;
}
bool FunctionManager::Edit(FunctionID id, const FunctionInfo& functionInfo)
{
	auto index = GetTemplateIndex(id);
	if(index != IndexNotFound)
	{
		g_FunctionTemplates[index] = functionInfo;
		return true;
	}
	return false;
}
const FunctionInfo& FunctionManager::Get(FunctionID id)
{
	auto index = GetTemplateIndex(id);
	if(index != IndexNotFound)
	{
		return g_FunctionTemplates[index];
	}
	else
	{
		static FunctionInfo dummy;
		return dummy;
	}
}

static int GetTemplateIndex(FunctionID id)
{
	// Start from estimated position
	int i = id - 1;
	if(id > g_FunctionTemplates.size())
	{
		i = g_FunctionTemplates.size()-1;
	}

	while(i >= 0)
	{
		if(g_FunctionTemplates[i].ID == id)
		{
			break;
		}
		else if(g_FunctionTemplates[i].ID < id)
		{
			return IndexNotFound;
		}

		--i;
	}

	return i;
}

} // namespace Koala::Utility::Core
