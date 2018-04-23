#ifndef KOALA__UTILITY__CORE__FUNCTION_MANAGER
#define KOALA__UTILITY__CORE__FUNCTION_MANAGER

#include <Koala/Utility/Core/Variable.h>
#include <string>
#include <vector>

namespace Koala::Utility::Core {

using FunctionID = size_t;

struct FunctionInfo
{
	FunctionID ID = 0;
	std::string Name;
	Text NameText = Text::Empty;

	std::vector<Variable> BackSlots;
	std::vector<Variable> FrontSlots;
};

class FunctionManager
{
public:
	static FunctionID Add(const FunctionInfo& functionInfo);
	static bool Remove(FunctionID id);
	static bool Edit(FunctionID id, const FunctionInfo& functionInfo);
	static const FunctionInfo& Get(FunctionID id);

};

} // namespace Koala::Utility::Core

#endif // KOALA__UTILITY__CORE__FUNCTION_MANAGER
