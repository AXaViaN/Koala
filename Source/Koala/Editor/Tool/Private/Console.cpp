#include <Koala/Editor/Tool/Private/Console.h>
#include <Koala/Editor/Tool/Input.h>
#include <Koala/Editor/Tool/Window.h>
#include <cstdarg>
#include <cstdio>
#include <vector>

namespace Koala::Editor::Tool {

// Static initialize
static Console& g_Console = Console::Instance();

static std::vector<Service::MessageType>& GetSpamMessages()
{
	static std::vector<Service::MessageType> spamMessages = {Service::MessageType::NodeMove, 
															 Service::MessageType::RequestNode};
	return spamMessages;
}
static void PrintMessageToConsole(const char* const format, ...)
{
	va_list argList;
	va_start(argList, format);

	static std::string lastMessage = std::string(256, '\0');
	static size_t repeatCount = 1;

	std::string currentMessage = std::string(256, '\0');
	std::vsprintf(&currentMessage[0], format, argList);

	if(currentMessage != lastMessage)
	{
		std::printf("\n%s", currentMessage.c_str());
		lastMessage = currentMessage;
		repeatCount = 1;
	}
	else
	{
		std::printf("\r%s (%zu)", currentMessage.c_str(), ++repeatCount);
	}

	va_end(argList);
}

void Console::OnMessage(Service::MessageType type, void* data)
{
#ifdef KOALA_DEBUG
	for( auto& spamMessage : GetSpamMessages() )
	{
		if(type == spamMessage)
		{
			return;
		}
	}

	PrintMessageToConsole("Message: %s", Service::MessageTypeToString(type).c_str());
#endif
}
void Console::OnInput(Service::InputMessageType type, const Service::InputMessageData& data)
{
#ifdef KOALA_DEBUG
	PrintMessageToConsole("Input: %s, %s    \t->\tKey=%s, Button=%s, Scroll=(%f, %f)", 
						  data.Window.GetTitle().c_str(), 
						  Service::InputMessageTypeToString(type).c_str(), 
						  KeyTypeToString(data.Key).c_str(), 
						  MouseButtonTypeToString(data.Button).c_str(), 
						  data.Scroll.GetX(), 
						  data.Scroll.GetY());
#endif
}

} // namespace Koala::Editor::Tool
