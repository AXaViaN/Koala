:orphan:

Koala::Editor::Tool::Window::(constructor)
==========================================

.. csv-table::
	
	"**Window();**", "*(1) Default*"
	"**Window(const std::string& title);**", "*(2) Maximized*"
	"**Window(const std::string& title, size_t width, size_t height);**", "*(3) Sized*"
	"**Window(const Window& other) = delete;**", "*(4) Copy*"
	"**Window(Window&& other) noexcept;**", "*(5) Move*"

*(1) Default constructor*
	Blank object; nothing is performed.

*(2) Maximized constructor*
	Calls :doc:`Window::Create <Window_Create>` to create a maximized window.

*(3) Sized constructor*
	Calls :doc:`Window::Create <Window_Create>` to create a window.

*(4) Copy constructor*
	Copy constructor is deleted. You need to create a new window.

*(5) Move constructor*
	Moves the window handle to this object.

----

Window constructors are calling member method :doc:`Window::Create <Window_Create>` to create a window. If you call the *(1) Default constructor*, you need to call :doc:`Window::Create <Window_Create>` explicitly.

Parameters
----------

- **title**
	Window title

- **width**
	Window width

- **height**
	Window height

See also
--------

- :doc:`../Window`

- :doc:`Window::(destructor) <Window_Dtor>`

- :doc:`Window::Create <Window_Create>`
