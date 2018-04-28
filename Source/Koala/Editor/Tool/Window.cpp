#include <Koala/Editor/Tool/Window.h>
#include <Koala/Editor/Tool/Input.h>
#include <Koala/Editor/Tool/PlatformManager.h>
#include <Koala/Editor/Service/MessageSender.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include <map>

namespace Koala::Editor::Tool {

// All of the windows will share the render context of one hidden window
static GLFWwindow* GetSharedContext();
// Input handling
static void GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void GlfwMousePositionCallback(GLFWwindow* window, double x, double y);
static void GlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
static void GlfwMouseScrollCallback(GLFWwindow* window, double x, double y);
// Helpers
static bool IsGlfwWindowRegistered(GLFWwindow* window);
static void LogUnknownGlfwAction(Service::LogMessageData data, int action);

// Collection to keep track of actual Window instances inside the GlfwKeyCallback
static std::map<GLFWwindow*, Window*> g_GlfwMap;

void Window::Create(const std::string& title)
{
	constexpr size_t DefaultWidth = 1280;
	constexpr size_t DefaultHeight = 720;
	Create(title, DefaultWidth, DefaultHeight);

	if(m_Handle == nullptr)
	{
		return;
	}

	glfwMaximizeWindow(static_cast<GLFWwindow*>(m_Handle));
}
void Window::Create(const std::string& title, size_t width, size_t height)
{
	const int WidthInt = static_cast<int>(width);
	const int HeightInt = static_cast<int>(height);

	// Destroy if there is already a window
	Destroy();

	m_Handle = glfwCreateWindow(WidthInt, HeightInt, title.c_str(), nullptr, GetSharedContext());
	Activate();

	if(m_Handle == nullptr)
	{
		return;
	}

	constexpr int EnableVSYNC = 1;
	glfwSwapInterval(EnableVSYNC);
	m_Title = title;
	g_GlfwMap[static_cast<GLFWwindow*>(m_Handle)] = this;

	// Set window to centered
	const GLFWvidmode* VideoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	const int ScreenWidth = VideoMode->width;
	const int ScreenHeight = VideoMode->height;

	int offsetX = (ScreenWidth - WidthInt) / 2;
	int offsetY = (ScreenHeight - HeightInt) / 2;
	glfwSetWindowPos(static_cast<GLFWwindow*>(m_Handle), offsetX, offsetY);

	// First initialize ImGui input handlers (2nd paramater)
	// and then override with own callback
	ImGui_ImplGlfwGL3_Init(static_cast<GLFWwindow*>(m_Handle), true);
	glfwSetKeyCallback(static_cast<GLFWwindow*>(m_Handle), GlfwKeyCallback);
	glfwSetCursorPosCallback(static_cast<GLFWwindow*>(m_Handle), GlfwMousePositionCallback);
	glfwSetMouseButtonCallback(static_cast<GLFWwindow*>(m_Handle), GlfwMouseButtonCallback);
	glfwSetScrollCallback(static_cast<GLFWwindow*>(m_Handle), GlfwMouseScrollCallback);
}
void Window::Destroy()
{
	if(m_Handle)
	{
		g_GlfwMap.erase(static_cast<GLFWwindow*>(m_Handle));
		glfwDestroyWindow(static_cast<GLFWwindow*>(m_Handle));
		m_Handle = nullptr;
		m_Title = "";
	}
}

void Window::Activate()
{
	glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_Handle));
}
void Window::Update()
{
	if(m_Handle == nullptr || 
	   glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Handle)))
	{
		Destroy();
		return;
	}

	int width;
	int height;
	glfwGetFramebufferSize(static_cast<GLFWwindow*>(m_Handle), &width, &height);
	glViewport(0, 0, width, height);

	glfwSwapBuffers(static_cast<GLFWwindow*>(m_Handle));
}

void Window::RenameTitle(const std::string& title)
{
	if(m_Handle == nullptr)
	{
		return;
	}

	glfwSetWindowTitle(static_cast<GLFWwindow*>(m_Handle), title.c_str());
	m_Title = title;
}

size_t Window::GetWidth() const
{
	if(m_Handle == nullptr)
	{
		return 0u;
	}

	int width;
	glfwGetFramebufferSize(static_cast<GLFWwindow*>(m_Handle), &width, nullptr);

	return width;
}
size_t Window::GetHeight() const
{
	if(m_Handle == nullptr)
	{
		return 0u;
	}

	int height;
	glfwGetFramebufferSize(static_cast<GLFWwindow*>(m_Handle), nullptr, &height);

	return height;
}

Window::~Window() noexcept
{
	Destroy();
}
Window::Window(Window&& other) noexcept : 
	m_Handle(other.m_Handle)
{
	other.m_Handle = nullptr;

	g_GlfwMap[static_cast<GLFWwindow*>(m_Handle)] = this;
}
Window& Window::operator=(Window&& other) noexcept
{
	if(this == &other)
	{
		return *this;
	}

	m_Handle = other.m_Handle;
	other.m_Handle = nullptr;

	g_GlfwMap[static_cast<GLFWwindow*>(m_Handle)] = this;

	return *this;
}

static GLFWwindow* GetSharedContext()
{
	// Create a hidden window for context sharing
	static GLFWwindow* s_MainWindowContext = nullptr;
	if(s_MainWindowContext == nullptr)
	{
		s_MainWindowContext = glfwCreateWindow(1, 1, "", nullptr, nullptr);
		if(s_MainWindowContext == nullptr)
		{
			return nullptr;
		}

		glfwMakeContextCurrent(s_MainWindowContext);

		bool success = Koala::Editor::Tool::PlatformManager::InitializeRenderContext();
		if(success == false)
		{
			glfwDestroyWindow(s_MainWindowContext);
			s_MainWindowContext = nullptr;
			return nullptr;
		}

		glfwHideWindow(s_MainWindowContext);
	}

	return s_MainWindowContext;
}

static void GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Input callback chain
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
	
	// If ImGui uses the input, we will ignore
	if(IsGlfwWindowRegistered(window) == false ||
	   ImGui::GetIO().WantCaptureKeyboard)
	{
		return;
	}

	Service::InputMessageType message;
	if(action == GLFW_PRESS)
	{
		message = Service::InputMessageType::KeyPress;
	}
	else if(action == GLFW_RELEASE)
	{
		message = Service::InputMessageType::KeyRelease;
	}
	else if(action == GLFW_REPEAT)
	{
		message = Service::InputMessageType::KeyHold;
	}
	else
	{
		auto data = GenerateLogMessageData();
		LogUnknownGlfwAction(data, action);

		return;
	}

	KeyType keyType = static_cast<KeyType>(key);
	
	Input::RegisterKey(*g_GlfwMap[window], message, keyType);
}
static void GlfwMousePositionCallback(GLFWwindow* window, double x, double y)
{
	if(IsGlfwWindowRegistered(window) == false)
	{
		return;
	}

	// Convert to the logical space
	auto& windowObject = *g_GlfwMap[window];
	x /= windowObject.GetWidth();
	y /= windowObject.GetHeight();
	
	Input::RegisterMousePosition(windowObject, Gfx::Vector2(static_cast<float>(x), static_cast<float>(y)));
}
static void GlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	// Input callback chain
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
	
	if(IsGlfwWindowRegistered(window) == false)
	{
		return;
	}

	Service::InputMessageType message;
	if(action == GLFW_PRESS)
	{
		message = Service::InputMessageType::MousePress;
	}
	else if(action == GLFW_RELEASE)
	{
		message = Service::InputMessageType::MouseRelease;
	}
	else
	{
		auto data = GenerateLogMessageData();
		LogUnknownGlfwAction(data, action);

		return;
	}
	
	MouseButtonType buttonType = MouseButtonType::Unknown;
	if(button == GLFW_MOUSE_BUTTON_LEFT)
	{
		buttonType = MouseButtonType::Left;
	}
	else if(button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		buttonType = MouseButtonType::Right;
	}
	else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		buttonType = MouseButtonType::Middle;
	}

	Input::RegisterMouseButton(*g_GlfwMap[window], message, buttonType);
}
static void GlfwMouseScrollCallback(GLFWwindow* window, double x, double y)
{
	// Input callback chain
	ImGui_ImplGlfw_ScrollCallback(window, x, y);

	if(IsGlfwWindowRegistered(window) == false)
	{
		return;
	}

	Input::RegisterMouseScroll(*g_GlfwMap[window], Gfx::Vector2(static_cast<float>(x), static_cast<float>(y)));
}

static bool IsGlfwWindowRegistered(GLFWwindow* window)
{
	if(g_GlfwMap.find(window) == g_GlfwMap.end())
	{
		auto data = GenerateLogMessageData();
		char msg[128];
		std::sprintf(msg, "Unregistered window: %p", window);
		data.Message = msg;
		Service::MessageSender::Send(Service::MessageType::LogError, &data);

		return false;
	}

	return true;
}
static void LogUnknownGlfwAction(Service::LogMessageData data, int action)
{
	data.Message = "Unknown glfw action: " + std::to_string(action);
	Service::MessageSender::Send(Service::MessageType::LogError, &data);
}

} // namespace Koala::Editor::Tool
