:orphan:

Koala::Editor::Tool::Window::Create
===================================

.. csv-table::
	
	"**void Create(const std::string& title);**", "*(1) Maximized*"
	"**void Create(const std::string& title, size_t width, size_t height);**", "*(2) Sized*"

*(1) Maximized*
	Calls *(2) Sized* version with default size of (1280, 720). Then sets window to maximized.

*(2) Sized*
	Creates vsync enabled window.

----

You can check the creation success with :doc:`Window::IsValid <Window_IsValid>`.

After the window is created, it is activated with member method :doc:`Window::Activate <Window_Activate>`. This makes this object as the active window and render target. :doc:`Window::Activate <Window_Activate>` is called even if the creation fails. This prevents accidental rendering to the last active window by making current render target invalid.

If this object already has a valid window handle, it is destroyed before creating the new window.

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

- :doc:`Window::Destroy <Window_Destroy>`

- :doc:`Window::Activate <Window_Activate>`

- :doc:`Window::IsValid <Window_IsValid>`
