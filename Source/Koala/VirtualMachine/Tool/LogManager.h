#ifndef KOALA__VIRTUAL_MACHINE__TOOL__LOG_MANAGER
#define KOALA__VIRTUAL_MACHINE__TOOL__LOG_MANAGER

#include <Koala/Utility/Resource.h>
#include <string>

#define ExitMessage(text, ...) \
	if(true) \
	{ \
		std::string message(512u, '\0'); \
		std::sprintf(&message[0], "[0x%p] %s", \
					 reinterpret_cast<void*>(text), Koala::Utility::Resource::GetText(text).c_str()); \
		std::string formatForArg = message; \
		std::sprintf(&message[0], formatForArg.c_str(), __VA_ARGS__); \
		message.erase(message.find_first_of('\0')); \
		\
		Koala::VirtualMachine::Tool::LogManager::Log(__FUNCTION__, size_t(__LINE__), message); \
	}

namespace Koala::VirtualMachine::Tool {

class LogManager
{
public:
	static void Log(const std::string& function, size_t line, const std::string& message);

};

} // namespace Koala::VirtualMachine::Tool

#endif // KOALA__VIRTUAL_MACHINE__TOOL__LOG_MANAGER
