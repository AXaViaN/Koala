:orphan:

Koala::Editor::Tool::Window::(operator)
=======================================

.. csv-table::
	
	"**Window& operator=(const Window& other) = delete;**", "*(1) Copy*"
	"**Window& operator=(Window&& other) noexcept;**", "*(2) Move*"

*(1) Copy assignment*
	Copy operator is deleted. You need to create a new window.

*(2) Move assignment*
	Moves the window handle to this object.

See also
--------

- :doc:`../Window`
