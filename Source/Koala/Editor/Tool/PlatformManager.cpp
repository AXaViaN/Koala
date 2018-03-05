#include <Koala/Editor/Tool/PlatformManager.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>

namespace Koala::Editor::Tool {

static void GlfwErrorCallback(int errorCode, const char* errorDesc);

bool PlatformManager::Initialize()
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if(glfwInit() == GLFW_FALSE)
	{
		std::printf("Glfw init error\n");
		std::getchar();
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
		std::printf("Glew init error\n");
		std::getchar();
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
	std::fprintf(stderr, "GlfwError: %d - %s\n", errorCode, errorDesc);
}

} // namespace Koala::Editor::Tool
