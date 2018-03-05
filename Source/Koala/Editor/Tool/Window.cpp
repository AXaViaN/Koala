#include <Koala/Editor/Tool/Window.h>
#include <Koala/Editor/Tool/PlatformManager.h>
#include <GLFW/glfw3.h>

namespace Koala::Editor::Tool {

// All of the windows will share the render context of one hidden window
static GLFWwindow* GetSharedContext();

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
	// Destroy if there is already a window
	Destroy();

	m_Handle = glfwCreateWindow(width, height, title.c_str(), nullptr, GetSharedContext());
	Activate();

	if(m_Handle == nullptr)
	{
		return;
	}

	constexpr int EnableVSYNC = 1;
	glfwSwapInterval(EnableVSYNC);

	// Set window to centered
	const GLFWvidmode* VideoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	const int ScreenWidth = VideoMode->width;
	const int ScreenHeight = VideoMode->height;

	int offsetX = (ScreenWidth - width) / 2;
	int offsetY = (ScreenHeight - height) / 2;
	glfwSetWindowPos(static_cast<GLFWwindow*>(m_Handle), offsetX, offsetY);
}
void Window::Destroy()
{
	if(m_Handle)
	{
		glfwDestroyWindow(static_cast<GLFWwindow*>(m_Handle));
		m_Handle = nullptr;
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
}
Window& Window::operator=(Window&& other) noexcept
{
	if(this == &other)
	{
		return *this;
	}

	m_Handle = other.m_Handle;
	other.m_Handle = nullptr;

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

} // namespace Koala::Editor::Tool
