#ifndef KOALA__UTILITY__CORE__SLOT
#define KOALA__UTILITY__CORE__SLOT

#include <Koala/Utility/Core/Variable.h>

namespace Koala::Utility::Core {

using NodeID = size_t;

struct Port
{
	NodeID NodeID = 0;
	size_t SlotIndex = 0;
};

class Slot
{
public:
	Slot(NodeID nodeID, size_t slotIndex, Variable variableTemplate) : 
		m_Port({nodeID, slotIndex}),
		m_Variable(variableTemplate)
	{ }

	void Connect(const Port& port)
	{
		m_Connection = port;
	}

	Variable& GetVariable()
	{
		return m_Variable;
	}
	const Port& GetPort() const
	{
		return m_Port;
	}

	const Port& GetConnection() const
	{
		return m_Connection;
	}
	bool IsConnected() const
	{
		return m_Connection.NodeID != 0;
	}

private:
	Port m_Port;
	Port m_Connection;

	Variable m_Variable;

};

} // namespace Koala::Utility::Core

#endif // KOALA__UTILITY__CORE__SLOT
