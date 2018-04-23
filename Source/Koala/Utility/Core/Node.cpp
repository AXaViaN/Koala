#include <Koala/Utility/Core/Node.h>
#include <Koala/Utility/IDGenerator.h>

namespace Koala::Utility::Core {

static IDGenerator g_IDGenerator;

Node::Node(FunctionID functionID) : 
	m_ID(g_IDGenerator.GetNextID()),
	m_FunctionID(functionID)
{
	auto& functionInfo = FunctionManager::Get(m_FunctionID);
	if(functionInfo.ID == 0)
	{
		return;
	}

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

} // namespace Koala::Utility::Core
