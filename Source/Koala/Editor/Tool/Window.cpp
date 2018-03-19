#include <Koala/Editor/Tool/Window.h>
#include <Koala/Editor/Tool/Input.h>
#include <Koala/Editor/Tool/PlatformManager.h>
#include <GLFW/glfw3.h>
#include <map>

namespace Koala::Editor::Tool {

// All of the windows will share the render context of one hidden window
static GLFWwindow* GetSharedContext();
// Input handling
static void GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
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
	glfwSetKeyCallback(static_cast<GLFWwindow*>(m_Handle), GlfwKeyCallback);
	m_Title = title;
	g_GlfwMap[static_cast<GLFWwindow*>(m_Handle)] = this;

	// Set window to centered
	const GLFWvidmode* VideoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	const int ScreenWidth = VideoMode->width;
	const int ScreenHeight = VideoMode->height;

	int offsetX = (ScreenWidth - WidthInt) / 2;
	int offsetY = (ScreenHeight - HeightInt) / 2;
	glfwSetWindowPos(static_cast<GLFWwindow*>(m_Handle), offsetX, offsetY);
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

static void GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int)
{
	if(g_GlfwMap.find(window) == g_GlfwMap.end())
	{
		std::printf("Unregistered window: %p\n", window);
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
		std::printf("Unknown glfw action: %d\n", action);
		return;
	}

	KeyType keyType = static_cast<KeyType>(key);
	
	Input::RegisterKeyInput(*g_GlfwMap[window], message, keyType);
}

} // namespace Koala::Editor::Tool
