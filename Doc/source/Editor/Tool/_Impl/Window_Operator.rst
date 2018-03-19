:orphan:

Koala::Editor::Tool::Window::(operator)
=======================================

.. csv-table::
	
	"**Window& operator=(const Window& other) = delete;**", "*(1) Copy*"
	"**Window& operator=(Window&& other) noexcept;**", "*(2) Move*"
	"**bool operator==(const Window& other) const;**", "*(3) Equality*"
	"**bool operator!=(const Window& other) const;**", "*(4) Inequality*"

*(1) Copy assignment*
	Copy operator is deleted. You need to create a new window.

*(2) Move assignment*
	Moves the window handle to this object.

*(3) Equality comparison && (4) Inequality comparison*
	Check if the handles are the same.

See also
--------

- :doc:`../Window`
