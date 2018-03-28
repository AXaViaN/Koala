#include <Koala/Utility/Logger.h>
#include <Koala/Utility/Clock.h>
#include <Koala/Utility/Date.h>
#include <cstdio>

namespace Koala::Utility {

void Logger::Log(const std::string& function, size_t line, LogType type, const std::string& message)
{
	if(m_LogFile.IsValid() == false)
	{
		return;
	}

	std::string logMessage = message + 
							 " {" + function + "::" + std::to_string(line) + "}" +
							 " [" + Clock::ProgramClock().ToString() + "]\r\n";

	if(type == LogType::Info)
	{
	#ifdef KOALA_DEBUG
		m_LogFile.Write("Info: " + logMessage);
	#endif
	}
	else if(type == LogType::Error)
	{
		m_LogFile.Write("Error: " + logMessage);
	}
}

Logger::Logger()
{
	std::string logMessage = "\r\n-------------------------------------------------------\r\n\r\n";
	logMessage += ProjectName() + " - " + Date::Now().ToString() + 
				  " " + Clock::Now().ToString() + "\r\n";

	if(m_LogFile.IsValid())
	{
		m_LogFile.Write(logMessage);
	}
}

} // namespace Koala::Utility
