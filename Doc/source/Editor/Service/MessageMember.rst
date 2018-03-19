MessageMember
=============

.. function:: class Koala::Editor::Service::MessageMember;

Interface for the message bus. Inherited classes could send and receive messages through this interface. Inputs are also handled with this interface. User inputs are sent by the :doc:`../Tool/Input` class.

This class creates a static :doc:`_Private/MessageManager` in the source file. Actual messaging is handled by that class.

Public Methods
--------------

.. csv-table::
	
	":doc:`(constructor) <_Impl/MessageMember_Ctor>`", "Constructors"
	":doc:`(destructor) <_Impl/MessageMember_Dtor>`", "Destructor"
	":doc:`(operator) <_Impl/MessageMember_Operator>`", "Copy, move operators"

.. csv-table::
	
	":doc:`OnMessage <_Impl/MessageMember_OnMessage>`", "Virtual message reciever"
	":doc:`OnInput <_Impl/MessageMember_OnInput>`", "Virtual input reciever"

Protected Methods
-----------------

.. csv-table::
	
	":doc:`SendMessage <_Impl/MessageMember_SendMessage>`", "Message sender"
	":doc:`SendInput <_Impl/MessageMember_SendInput>`", "Input sender"
