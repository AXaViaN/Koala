#include <Koala/Editor/Tool/Private/Console.h>
#include <Koala/Editor/Tool/Window.h>
#include <cstdio>

namespace Koala::Editor::Tool {

// Static initialize
static Console& g_Console = Console::Instance();

void Console::OnMessage(Service::MessageType type, void* data)
{
#ifdef KOALA_DEBUG
	std::printf("Console: Message sent = %d\n", type);
#endif
}
void Console::OnInput(Service::InputMessageType type, const Service::InputMessageData& data)
{
#ifdef KOALA_DEBUG
	std::printf("Console: Input sent = %s, %d, %d\n", data.Window.GetTitle().c_str(), type, data.Key);
#endif
}

} // namespace Koala::Editor::Tool
