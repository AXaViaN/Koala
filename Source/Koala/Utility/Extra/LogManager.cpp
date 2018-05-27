#include <Koala/Utility/Extra/LogManager.h>
#include <Koala/Utility/Logger.h>

namespace Koala::Utility::Extra {

void LogManager::Initialize(const std::string& projectName)
{
	Logger::Initialize(projectName);
	Resource::Initialize();
}

void LogManager::Log(const std::string& function, size_t line, const std::string& message)
{
	// Write to the log file
	Logger::Instance().Log(function, line, LogType::Error, message);

	// Print to the console
	std::printf("\n\n-------------------------\n\n");
	std::printf("%s\n", message.c_str());
	std::printf("%s\n", Resource::GetText(Text::PressEnterToExit).c_str());
	std::getchar();
}

} // namespace Koala::Utility::Extra
