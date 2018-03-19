:orphan:

Koala::Editor::Service::MessageManager::AddMember
=================================================

.. csv-table::
	
	"**void AddMember(MessageMember* member);**"

Register member to message list. This is called from :doc:`../../MessageMember` constructors.

This method doesn't check if the object is registered before. So don't call this twice on the same object. Any duplication in the list would create UB.

Parameters
----------

- **member**
	MessageMember to register

See also
--------

- :doc:`../MessageManager`

- :doc:`../../MessageMember`

- :doc:`MessageManager::RemoveMember <MessageManager_RemoveMember>`
