#include <Koala/Editor/Tool/Input.h>
#include <GLFW/glfw3.h>

namespace Koala::Editor::Tool {

static Gfx::Vector2 g_MousePosition;
// Manually handle mouse button holding, because glfw doesn't
static bool g_IsHoldingMouseButton = false;
static MouseButtonType g_HoldingMouseButton;
static const Tool::Window* g_HoldingMouseButtonWindow;

void Input::Update()
{
	glfwPollEvents();

	if(g_IsHoldingMouseButton)
	{
		static Input instance;
		instance.SendInput(Service::InputMessageType::MouseHold, 
						   {*g_HoldingMouseButtonWindow, KeyType::Unknown, g_HoldingMouseButton});
	}
}

const Gfx::Vector2& Input::GetMousePosition()
{
	return g_MousePosition;
}

void Input::RegisterKey(const Window& window, Service::InputMessageType message, KeyType key)
{
	static Input instance;

	Service::InputMessageData data = {window, key, MouseButtonType::Unknown};
	instance.SendInput(message, data);
}
void Input::RegisterMousePosition(const Window& window, Gfx::Vector2 position)
{
	// Mouse position is not sent as an input message
	g_MousePosition = position;
}
void Input::RegisterMouseButton(const Window& window, Service::InputMessageType message, MouseButtonType button)
{
	static Input instance;

	Service::InputMessageData data = {window, KeyType::Unknown, button};
	instance.SendInput(message, data);

	// Manually handle mouse button holding, because glfw doesn't
	if(message == Service::InputMessageType::MousePress)
	{
		g_IsHoldingMouseButton = true;
		g_HoldingMouseButton = button;
		g_HoldingMouseButtonWindow = &window;
	}
	else if(message == Service::InputMessageType::MouseRelease)
	{
		g_IsHoldingMouseButton = false;
	}
}
void Input::RegisterMouseScroll(const Window& window, Gfx::Vector2 scroll)
{
	static Input instance;

	Service::InputMessageData data = {window, KeyType::Unknown, MouseButtonType::Unknown, scroll};
	instance.SendInput(Service::InputMessageType::MouseScroll, data);
}

} // namespace Koala::Editor::Tool
