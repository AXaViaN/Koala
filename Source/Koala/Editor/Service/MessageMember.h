#ifndef KOALA__EDITOR__SERVICE__MESSAGE_BUS
#define KOALA__EDITOR__SERVICE__MESSAGE_BUS

#include <Koala/Editor/Service/MessageTypes.h>

namespace Koala::Editor::Service {

class MessageMember
{
public:
	MessageMember();

	virtual void OnMessage(MessageType type, void* data) = 0;
	virtual void OnInput(InputMessageType type, const InputMessageData& data) = 0;

public: // Rule of five
	~MessageMember() noexcept;
	MessageMember(const MessageMember& other);
	MessageMember(MessageMember&& other) noexcept;
	MessageMember& operator=(const MessageMember& other);
	MessageMember& operator=(MessageMember&& other) noexcept;

protected:
	void SendMessage(MessageType type, void* data);
	void SendInput(InputMessageType type, const Service::InputMessageData& data);

};

} // namespace Koala::Editor::Service

#endif // KOALA__EDITOR__SERVICE__MESSAGE_BUS
