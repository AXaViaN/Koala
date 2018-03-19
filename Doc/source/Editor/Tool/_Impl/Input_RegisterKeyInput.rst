:orphan:

Koala::Editor::Tool::Input::RegisterKeyInput
============================================

.. csv-table::
	
	"**static void RegisterKeyInput(const Window& window, Service::InputMessageType message, KeyType key);**"

Get inputs and redistribute them as an input message. This method is called by :doc:`../Window` class whenever an input happens.

Parameters
----------

- **window**
	The window that the input occured

- **message**
	Input type

- **key**
	Key type

See also
--------

- :doc:`../Input`

- :doc:`../Window`
