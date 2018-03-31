#ifndef KOALA__EDITOR__SERVICE__MESSAGE_SENDER
#define KOALA__EDITOR__SERVICE__MESSAGE_SENDER

#include <Koala/Editor/Service/MessageMember.h>

namespace Koala::Editor::Service {

class MessageSender : private Service::MessageMember
{
public:
	static void Send(MessageType type, void* data)
	{
		static MessageSender instance;
		instance.SendMessage(type, data);
	}

private:
	MessageSender()
	{ }

	virtual void OnMessage(MessageType type, void* data)
	{ }
	virtual void OnInput(InputMessageType type, const InputMessageData& data)
	{ }

};

} // namespace Koala::Editor::Service

#endif // KOALA__EDITOR__SERVICE__MESSAGE_SENDER
