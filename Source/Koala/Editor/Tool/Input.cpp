#include <Koala/Editor/Tool/Input.h>
#include <GLFW/glfw3.h>

namespace Koala::Editor::Tool {

void Input::Update()
{
	glfwPollEvents();
}

void Input::RegisterKeyInput(const Window& window, Service::InputMessageType message, KeyType key)
{
	static Input instance;

	Service::InputMessageData data = {window, key};
	instance.SendInput(message, data);
}

} // namespace Koala::Editor::Tool
