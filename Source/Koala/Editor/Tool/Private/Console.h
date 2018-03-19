#ifndef PRIVATE__KOALA__EDITOR__TOOL__CONSOLE
#define PRIVATE__KOALA__EDITOR__TOOL__CONSOLE

#include <Koala/Editor/Service/MessageMember.h>

namespace Koala::Editor::Tool {

class Console : private Service::MessageMember
{
public:
	virtual void OnMessage(Service::MessageType type, void* data) override;
	virtual void OnInput(Service::InputMessageType type, const Service::InputMessageData& data) override;

	static Console& Instance()
	{
		static Console instance;
		return instance;
	}

private:
	Console()
	{ }
	~Console()
	{ }
	Console(const Console& other) = delete;
	Console(Console&& other) = delete;
	Console& operator=(const Console& other) = delete;
	Console& operator=(Console&& other) = delete;

};

} // namespace Koala::Editor::Tool

#endif // PRIVATE__KOALA__EDITOR__TOOL__CONSOLE
