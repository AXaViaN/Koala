#include <Koala/Editor/Service/Private/MessageManager.h>
#include <Koala/Editor/Service/MessageMember.h>

namespace Koala::Editor::Service {

void MessageManager::AddMember(MessageMember* member)
{
	m_Members.push_back(member);
}
void MessageManager::RemoveMember(MessageMember* member)
{
	m_Members.remove_if(
		[member](MessageMember* mem) -> bool
		{
			return (mem == member);
		});
}

void MessageManager::BroadcastMessage(MessageMember* sender, MessageType type, void* data) const
{
	for( auto& member : m_Members )
	{
		if(member != sender)
		{
			member->OnMessage(type, data);
		}
	}
}
void MessageManager::BroadcastInput(MessageMember* sender, InputMessageType type, const Service::InputMessageData& data) const
{
	for( auto& member : m_Members )
	{
		if(member != sender)
		{
			member->OnInput(type, data);
		}
	}
}

} // namespace Koala::Editor::Service
