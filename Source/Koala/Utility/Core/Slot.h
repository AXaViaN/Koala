#ifndef KOALA__UTILITY__CORE__SLOT
#define KOALA__UTILITY__CORE__SLOT

#include <Koala/Utility/Core/Variable.h>
#include <vector>

namespace Koala::Utility {
class Serialization;
}

namespace Koala::Utility::Core {

using NodeID = size_t;

struct Port
{
	NodeID NodeID = 0;
	size_t SlotIndex = 0;
};

class Slot
{
	friend class Serialization;
public:
	Slot(NodeID nodeID, size_t slotIndex, Variable variableTemplate) : 
		m_Port({nodeID, slotIndex}),
		m_Variable(variableTemplate)
	{ }

	void Connect(Slot& slot)
	{
		this->m_Connections.emplace_back(slot.m_Port);
		slot.m_Connections.emplace_back(this->m_Port);
	}
	void Disconnect(const Port& port)
	{
		for( size_t i=0 ; i<m_Connections.size() ; ++i )
		{
			if(m_Connections[i].NodeID == port.NodeID &&
			   m_Connections[i].SlotIndex == port.SlotIndex)
			{
				m_Connections.erase(m_Connections.begin() + i);
				break;
			}
		}
	}
	void DisconnectAll()
	{
		m_Connections.clear();
	}

	Variable& GetVariable()
	{
		return m_Variable;
	}
	const Variable& GetVariable() const
	{
		return m_Variable;
	}
	const Port& GetPort() const
	{
		return m_Port;
	}

	const std::vector<Port>& GetConnections() const
	{
		return m_Connections;
	}
	const Port& GetConnection(const Port& port) const
	{
		for( auto& connection : m_Connections )
		{
			if(connection.NodeID == port.NodeID &&
			   connection.SlotIndex == port.SlotIndex)
			{
				return connection;
			}
		}

		static Port dummy;
		return dummy;
	}
	bool IsConnected() const
	{
		return m_Connections.size() != 0;
	}
	size_t GetConnectionCount() const
	{
		return m_Connections.size();
	}

private:
	Port m_Port;
	std::vector<Port> m_Connections;

	Variable m_Variable;

};

} // namespace Koala::Utility::Core

#endif // KOALA__UTILITY__CORE__SLOT
