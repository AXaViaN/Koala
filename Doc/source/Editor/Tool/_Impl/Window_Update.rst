:orphan:

Koala::Editor::Tool::Window::Update
===================================

.. csv-table::
	
	"**void Update();**"

Swap buffers and update validation. If the user clicked to *X* button of the window, the window will be destroyed.

API user is expected to call this method on every frame. It is safe to call this method on invalid or already destroyed window.

See also
--------

- :doc:`../Window`
