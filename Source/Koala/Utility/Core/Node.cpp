#include <Koala/Utility/Core/Node.h>
#include <Koala/Utility/IDGenerator.h>

namespace Koala::Utility::Core {

static IDGenerator g_IDGenerator;

Node::Node(FunctionID functionID) : 
	m_FunctionID(functionID)
{
	auto& functionInfo = FunctionManager::Get(m_FunctionID);
	if(functionInfo.ID == 0)
	{
		m_ID = 0;
		return;
	}

	m_ID = g_IDGenerator.GetNextID();

	for( size_t i=0 ; i<functionInfo.BackSlots.size() ; ++i )
	{
		m_BackSlots.emplace_back(m_ID, i, functionInfo.BackSlots[i]);
	}
	for( size_t i=0 ; i<functionInfo.FrontSlots.size() ; ++i )
	{
		m_FrontSlots.emplace_back(m_ID, i, functionInfo.FrontSlots[i]);
	}
}

const std::string& Node::GetName() const
{
	auto& functionInfo = FunctionManager::Get(m_FunctionID);
	if(functionInfo.ID == 0)
	{
		static std::string dummy;
		return dummy;
	}

	if(functionInfo.NameText == Text::Empty)
	{
		return functionInfo.Name;
	}
	else
	{
		return Resource::GetText(functionInfo.NameText);
	}
}
const std::vector<Slot>& Node::GetBackSlots() const
{
	return m_BackSlots;
}
const std::vector<Slot>& Node::GetFrontSlots() const
{
	return m_FrontSlots;
}

std::vector<Slot>& Node::GetBackSlots()
{
	return m_BackSlots;
}
std::vector<Slot>& Node::GetFrontSlots()
{
	return m_FrontSlots;
}

const std::vector<Slot>& Node::GetSlots(SlotSide slotSide) const
{
	if(slotSide == SlotSide::Front)
	{
		return GetFrontSlots();
	}
	else
	{
		return GetBackSlots();
	}
}
std::vector<Slot>& Node::GetSlots(SlotSide slotSide)
{
	if(slotSide == SlotSide::Front)
	{
		return GetFrontSlots();
	}
	else
	{
		return GetBackSlots();
	}
}

NodeID Node::PeekNextID()
{
	NodeID nextID = g_IDGenerator.GetNextID();
	g_IDGenerator.SetNextID(nextID);
	return nextID;
}
void Node::SetNextID(NodeID id)
{
	g_IDGenerator.SetNextID(id);
}

} // namespace Koala::Utility::Core
