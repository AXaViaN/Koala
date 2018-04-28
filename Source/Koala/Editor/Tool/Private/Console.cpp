#include <Koala/Editor/Tool/Private/Console.h>
#include <Koala/Editor/Tool/Input.h>
#include <Koala/Editor/Tool/Window.h>
#include <cstdio>

namespace Koala::Editor::Tool {

// Static initialize
static Console& g_Console = Console::Instance();

void Console::OnMessage(Service::MessageType type, void* data)
{
#ifdef KOALA_DEBUG
	std::printf("Message: %s\n", Service::MessageTypeToString(type).c_str());
#endif
}
void Console::OnInput(Service::InputMessageType type, const Service::InputMessageData& data)
{
#ifdef KOALA_DEBUG
	std::printf("Input: %s, %s    \t->\tKey=%s, Button=%s, Scroll=(%f, %f)\n", 
				data.Window.GetTitle().c_str(), 
				Service::InputMessageTypeToString(type).c_str(), 
				KeyTypeToString(data.Key).c_str(), 
				MouseButtonTypeToString(data.Button).c_str(), 
				data.Scroll.GetX(), 
				data.Scroll.GetY());
#endif
}

} // namespace Koala::Editor::Tool
