#ifndef KOALA__EDITOR__TOOL__PLATFORM_MANAGER
#define KOALA__EDITOR__TOOL__PLATFORM_MANAGER

#include <Koala/Editor/Service/MessageMember.h>

namespace Koala::Editor::Tool {

class PlatformManager : private Service::MessageMember
{
public:
	static bool Initialize();
	static bool InitializeRenderContext();

	static void Terminate();

private:
	PlatformManager()
	{ }

	static PlatformManager& Instance()
	{
		static PlatformManager instance;
		return instance;
	}

	virtual void OnMessage(Service::MessageType type, void* data) override
	{ }
	virtual void OnInput(Service::InputMessageType type, const Service::InputMessageData& data) override
	{ }

};

} // namespace Koala::Editor::Tool

#endif // KOALA__EDITOR__TOOL__PLATFORM_MANAGER
