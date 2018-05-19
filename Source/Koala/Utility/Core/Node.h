#ifndef KOALA__UTILITY__CORE__NODE
#define KOALA__UTILITY__CORE__NODE

#include <Koala/Utility/Core/FunctionManager.h>
#include <Koala/Utility/Core/Slot.h>
#include <vector>

namespace Koala::Utility {
class Serialization;
}

namespace Koala::Utility::Core {

using NodeID = size_t;

enum class SlotSide
{
	Front,
	Back
};

class Node
{
	friend class Serialization;
public:
	Node(FunctionID functionID);

	const std::string& GetName() const;
	const std::vector<Slot>& GetBackSlots() const;
	const std::vector<Slot>& GetFrontSlots() const;

	std::vector<Slot>& GetBackSlots();
	std::vector<Slot>& GetFrontSlots();

	const std::vector<Slot>& GetSlots(SlotSide slotSide) const;
	std::vector<Slot>& GetSlots(SlotSide slotSide);
	
	const NodeID& GetID() const
	{
		return m_ID;
	}

	// Serialization Helpers
private:
	static NodeID PeekNextID();
	static void SetNextID(NodeID id);

private:
	NodeID m_ID = 0;
	FunctionID m_FunctionID = 0;

	std::vector<Slot> m_BackSlots;
	std::vector<Slot> m_FrontSlots;
	
};

} // namespace Koala::Utility::Core

#endif // KOALA__UTILITY__CORE__NODE
