:orphan:

Koala::Editor::Tool::Window::IsValid
====================================

.. csv-table::
	
	"**bool IsValid() const;**"

Check if the window is valid. Active window is not important, this method just checks window handle.

Return Value
------------

If the object has valid window, it returns :code:`true`. If the window creation is failed or the window is destroyed, it returns :code:`false`.

See also
--------

- :doc:`../Window`

- :doc:`Window::Create <Window_Create>`

- :doc:`Window::Destroy <Window_Destroy>`
