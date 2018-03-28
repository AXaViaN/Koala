#include <Koala/Editor/Tool/Private/LogManager.h>
#include <Koala/Utility/Logger.h>

namespace Koala::Editor::Tool {

// Static initialize
static LogManager& g_LogManager = LogManager::Instance();

void LogManager::OnMessage(Service::MessageType type, void* data)
{
	if(data == nullptr)
	{
		return;
	}

	auto logData = static_cast<Service::LogMessageData*>(data);
	switch(type)
	{
		case Service::MessageType::LogInfo:
		{
			Utility::Logger::Instance().Log(logData->Function, logData->Line, 
											Utility::LogType::Info, logData->Message);
			break;
		}
		case Service::MessageType::LogError:
		{
			Utility::Logger::Instance().Log(logData->Function, logData->Line, 
											Utility::LogType::Error, logData->Message);
			break;
		}
	}
}

LogManager::LogManager()
{
	Utility::Logger::Initialize("Koala Editor");
}

} // namespace Koala::Editor::Tool
