#ifndef KOALA__EDITOR__TOOL__WINDOW
#define KOALA__EDITOR__TOOL__WINDOW

#include <string>

namespace Koala::Editor::Tool {

class Window
{
public:
	Window()
	{ }
	Window(const std::string& title)
	{
		Create(title);
	}
	Window(const std::string& title, size_t width, size_t height)
	{
		Create(title, width, height);
	}

	void Create(const std::string& title);
	void Create(const std::string& title, size_t width, size_t height);
	void Destroy();

	void Activate();
	void Update();

	void RenameTitle(const std::string& title);

	size_t GetWidth() const;
	size_t GetHeight() const;
	const std::string& GetTitle() const
	{
		return m_Title;
	}

	bool IsValid() const
	{
		return (m_Handle != nullptr);
	}

	bool operator==(const Window& other) const
	{
		return (m_Handle == other.m_Handle);
	}
	bool operator!=(const Window& other) const
	{
		return (m_Handle != other.m_Handle);
	}

public: // Rule of five
	~Window() noexcept;
	Window(const Window& other) = delete;
	Window(Window&& other) noexcept;
	Window& operator=(const Window& other) = delete;
	Window& operator=(Window&& other) noexcept;

private:
	void* m_Handle = nullptr;
	std::string m_Title;

};

} // namespace Koala::Editor::Tool

#endif // KOALA__EDITOR__TOOL__WINDOW
