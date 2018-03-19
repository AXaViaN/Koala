#include <Koala/Editor/Service/MessageMember.h>
#include <Koala/Editor/Service/Private/MessageManager.h>

namespace Koala::Editor::Service {

// Actual message bus
static MessageManager g_MessageManager;

MessageMember::MessageMember()
{
	g_MessageManager.AddMember(this);
}

void MessageMember::SendMessage(MessageType type, void* data)
{
	g_MessageManager.BroadcastMessage(this, type, data);
}
void MessageMember::SendInput(InputMessageType type, const Service::InputMessageData& data)
{
	g_MessageManager.BroadcastInput(this, type, data);
}

MessageMember::~MessageMember() noexcept
{
	g_MessageManager.RemoveMember(this);
}
MessageMember::MessageMember(const MessageMember& other)
{
	g_MessageManager.AddMember(this);
}
MessageMember::MessageMember(MessageMember&& other) noexcept
{
	g_MessageManager.AddMember(this);
}
MessageMember& MessageMember::operator=(const MessageMember& other)
{
	return *this;
}
MessageMember& MessageMember::operator=(MessageMember&& other) noexcept
{
	return *this;
}

} // namespace Koala::Editor::Service
