:orphan:

InputMessageType
================

.. function:: enum class Koala::Editor::Service::InputMessageType;

Enumeration for input messages. This enum is created with macros that provides string versions of the values.

.. csv-table::
	
	KeyPress, "A key from keyboard is pressed"
	KeyRelease, "A key from keyboard is released"
	KeyHold, "A key from keyboard is holding down"

Example
-------

	.. code-block:: c++
		
		void PrintInputType(InputMessageType inputType)
		{
		    std::printf("%s\n", InputMessageTypeToString(inputType).c_str());
		}

See also
--------

- :doc:`../MessageTypes`
