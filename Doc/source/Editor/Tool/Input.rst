Input
=====

.. function:: class Koala::Editor::Tool::Input final : private Service::MessageMember;

This header also has an enumeration: :doc:`KeyType <_Impl/Input_KeyType>`.

----

Input class is an abstraction for all types of user input. It polls new inputs with the :doc:`Update <_Impl/Input_Update>` method. But the actual inputs are recieved from the :doc:`Window` class via :doc:`RegisterKeyInput <_Impl/Input_RegisterKeyInput>` method.

Input initializes itself in the source file when it needs to send messages. Since no one needs to initialize this class, its constructor is private.

Public Methods
--------------

.. csv-table::
	
	":doc:`Update <_Impl/Input_Update>`", "Poll new inputs"
	":doc:`RegisterKeyInput <_Impl/Input_RegisterKeyInput>`", "Get key inputs and redistribute as message"

Public Overrides
----------------

.. csv-table::
	
	"OnMessage", "Empty message listener"
	"OnInput", "Empty input listener"

Private Methods
---------------

.. csv-table::
	
	"(constructor)", "Empty constructor and deleted copy, move"
	"(destructor)", "Empty destructor"
	"(operator)", "Deleted copy, move operators"

See also
--------

- :doc:`Window`

- :doc:`../Service/MessageMember`
