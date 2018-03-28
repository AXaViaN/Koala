:orphan:

MessageType
===========

.. function:: enum class Koala::Editor::Service::MessageType;

Enumeration for messages. This enum is created with macros that provides string versions of the values.

.. csv-table::
	
	ProgramExit, "Program is about to be terminated"
	LogInfo, "Log debug info to the user log file"
	LogError, "Log error to the user log file"

Example
-------

	.. code-block:: c++
		
		void PrintMessageType(MessageType messageType)
		{
		    std::printf("%s\n", MessageTypeToString(messageType).c_str());
		}

See also
--------

- :doc:`../MessageTypes`
