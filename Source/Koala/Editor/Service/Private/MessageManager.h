#ifndef PRIVATE__KOALA__EDITOR__SERVICE__MESSAGE_MANAGER
#define PRIVATE__KOALA__EDITOR__SERVICE__MESSAGE_MANAGER

#include <Koala/Editor/Service/MessageTypes.h>
#include <list>

namespace Koala::Editor::Service {

class MessageMember;

class MessageManager final
{
public:
	void AddMember(MessageMember* member);
	void RemoveMember(MessageMember* member);

	void BroadcastMessage(MessageMember* sender, MessageType type, void* data) const;
	void BroadcastInput(MessageMember* sender, InputMessageType type, const Service::InputMessageData& data) const;

private:
	std::list<MessageMember*> m_Members;

};

} // namespace Koala::Editor::Service

#endif // PRIVATE__KOALA__EDITOR__SERVICE__MESSAGE_MANAGER
