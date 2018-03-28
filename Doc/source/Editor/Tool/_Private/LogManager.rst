LogManager
==========

.. function:: class Koala::Editor::Tool::LogManager : private Service::MessageMember;

Initializes the :code:`Utility::Logger` and listens the message bus to redirect the logging messages to it.

LogManager statically initializes itself in the source file. Since it is only listening the message bus, no one needs to include this class.

Public Methods
--------------

.. csv-table::
	
	"Instance", "Singleton instance"

Public Overrides
----------------

.. csv-table::
	
	"OnMessage", "Message listener for logging"
	"OnInput", "Empty input listener"

Private Methods
---------------

.. csv-table::
	
	"(constructor)", "Constructor initializes the logger and deleted copy, move"
	"(destructor)", "Empty destructor"
	"(operator)", "Deleted copy, move operators"

See also
--------

- :doc:`../../Service/MessageMember`
