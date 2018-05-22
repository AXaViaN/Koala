#include <Koala/VirtualMachine/Tool/LogManager.h>
#include <Koala/Utility/Logger.h>

namespace Koala::VirtualMachine::Tool {

static char Initialize();
static char g_DummyInitializer = Initialize();

void LogManager::Log(const std::string& function, size_t line, const std::string& message)
{
	// Write to the log file
	Utility::Logger::Instance().Log(function, line, Utility::LogType::Error, message);

	// Print to the console
	std::printf("\n\n-------------------------\n\n");
	std::printf("%s\n", message.c_str());
	std::printf("%s\n", Utility::Resource::GetText(Utility::Text::VM_PressEnterToExit).c_str());
	std::getchar();
}

static char Initialize()
{
	Utility::Logger::Initialize("Koala VirtualMachine");
	Koala::Utility::Resource::Initialize();

	return '\0';
}

} // namespace Koala::VirtualMachine::Tool
