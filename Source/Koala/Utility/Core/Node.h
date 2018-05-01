#ifndef KOALA__UTILITY__CORE__NODE
#define KOALA__UTILITY__CORE__NODE

#include <Koala/Utility/Core/FunctionManager.h>
#include <Koala/Utility/Core/Slot.h>
#include <vector>

namespace Koala::Utility::Core {

using NodeID = size_t;

class Node
{
public:
	Node(FunctionID functionID);

	const std::string& GetName() const;
	const std::vector<Slot>& GetBackSlots() const;
	const std::vector<Slot>& GetFrontSlots() const;

	std::vector<Slot>& GetBackSlots();
	std::vector<Slot>& GetFrontSlots();

	const NodeID& GetID()
	{
		return m_ID;
	}

private:
	NodeID m_ID;
	FunctionID m_FunctionID;

	std::vector<Slot> m_BackSlots;
	std::vector<Slot> m_FrontSlots;
	
};

} // namespace Koala::Utility::Core

#endif // KOALA__UTILITY__CORE__NODE
