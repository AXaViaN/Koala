#include <Koala/Editor/Tool/PlatformManager.h>
#include <Koala/Editor/Service/MessageSender.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Koala::Editor::Tool {

static void GlfwErrorCallback(int errorCode, const char* errorDesc);

bool PlatformManager::Initialize()
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if(glfwInit() == GLFW_FALSE)
	{
		auto messageData = GenerateLogMessageData();
		messageData.Message = "Glfw init error";
		Instance().SendMessage(Service::MessageType::LogError, &messageData);

		return false;
	}

	// Setup glfw window context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return true;
}
bool PlatformManager::InitializeRenderContext()
{
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		auto messageData = GenerateLogMessageData();
		messageData.Message = "Glew init error";
		Instance().SendMessage(Service::MessageType::LogError, &messageData);

		return false;
	}

	return true;
}

void PlatformManager::Terminate()
{
	glfwTerminate();
}

static void GlfwErrorCallback(int errorCode, const char* errorDesc)
{
	auto messageData = GenerateLogMessageData();
	messageData.Message = "GlfwError: " + std::to_string(errorCode) + " - " + errorDesc;
	Service::MessageSender::Send(Service::MessageType::LogError, &messageData);
}

} // namespace Koala::Editor::Tool
