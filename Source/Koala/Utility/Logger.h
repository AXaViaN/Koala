#ifndef KOALA__UTILITY__LOGGER
#define KOALA__UTILITY__LOGGER

#include <Koala/Utility/File.h>
#include <string>

#define LogInfo(message) \
	Koala::Utility::Logger::Instance().Log(__FUNCTION__, size_t(__LINE__), Utility::LogType::Info, message)
#define LogError(message) \
	Koala::Utility::Logger::Instance().Log(__FUNCTION__, size_t(__LINE__), Utility::LogType::Error, message)

namespace Koala::Utility {

enum class LogType;

class Logger
{
public:
	static void Initialize(const std::string& projectName)
	{
		ProjectName() = projectName;
		Instance();
	}
	static Logger& Instance()
	{
		static Logger instance;
		return instance;
	}

	void Log(const std::string& function, size_t line, LogType type, const std::string& message);

private:
	Logger();

	static std::string& ProjectName()
	{
		static std::string s_ProjectName;
		return s_ProjectName;
	}

private:
	const std::string LogFilePath = "Data/user.log";

	File m_LogFile = File(LogFilePath);
	
};

enum class LogType
{
	Info,
	Error
};

} // namespace Koala::Utility

#endif // KOALA__UTILITY__LOGGER
