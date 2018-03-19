MessageManager
==============

.. function:: class Koala::Editor::Service::MessageManager final;

MessageManager class manages the actual message bus. It has a list of :doc:`../MessageMember` objects. Members register/unregister themselves to this list in their constructors and destructors. When a member sends a message, it is broadcast from this class.

Public Methods
--------------

.. csv-table::
	
	":doc:`AddMember <_Impl/MessageManager_AddMember>`", "Register member to message list"
	":doc:`RemoveMember <_Impl/MessageManager_RemoveMember>`", "Unregister member from message list"
	":doc:`BroadcastMessage <_Impl/MessageManager_BroadcastMessage>`", "Send message to everyone"
	":doc:`BroadcastInput <_Impl/MessageManager_BroadcastInput>`", "Send input to everyone"

Private Members
---------------

.. csv-table::
	
	"m_Members", "List of :doc:`../MessageMember` pointers"
