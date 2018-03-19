Console
=======

.. function:: class Koala::Editor::Tool::Console : private Service::MessageMember;

Listens the message bus and prints the messages to the console. This only happens if the program is in debug mode.

Console statically initializes itself in the source file. Since it is only listening the message bus, no one needs to include this class.

Public Methods
--------------

.. csv-table::
	
	"Instance", "Singleton instance"

Public Overrides
----------------

.. csv-table::
	
	"OnMessage", "Message listener"
	"OnInput", "Input listener"

Private Methods
---------------

.. csv-table::
	
	"(constructor)", "Empty constructor and deleted copy, move"
	"(destructor)", "Empty destructor"
	"(operator)", "Deleted copy, move operators"

See also
--------

- :doc:`../../Service/MessageMember`
