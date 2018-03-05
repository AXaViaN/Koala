:orphan:

Koala::Editor::Tool::Window::Activate
=====================================

.. csv-table::
	
	"**void Activate();**"

This method makes this object as the active window and render target. If the window is invalid, current render target will become invalid and render calls will not effect any window. This prevents accidental rendering to the last active window. 

See also
--------

- :doc:`../Window`
