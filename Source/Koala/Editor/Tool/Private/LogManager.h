#ifndef PRIVATE__KOALA__EDITOR__TOOL__LOG_MANAGER
#define PRIVATE__KOALA__EDITOR__TOOL__LOG_MANAGER

#include <Koala/Editor/Service/MessageMember.h>

namespace Koala::Editor::Tool {

class LogManager : private Service::MessageMember
{
public:
	virtual void OnMessage(Service::MessageType type, void* data) override;
	virtual void OnInput(Service::InputMessageType type, const Service::InputMessageData& data) override
	{ }

	static LogManager& Instance()
	{
		static LogManager instance;
		return instance;
	}

private:
	LogManager();
	~LogManager()
	{ }
	LogManager(const LogManager& other) = delete;
	LogManager(LogManager&& other) = delete;
	LogManager& operator=(const LogManager& other) = delete;
	LogManager& operator=(LogManager&& other) = delete;

};

} // namespace Koala::Editor::Tool

#endif // PRIVATE__KOALA__EDITOR__TOOL__LOG_MANAGER
