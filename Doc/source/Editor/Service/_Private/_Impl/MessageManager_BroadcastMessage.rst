:orphan:

Koala::Editor::Service::MessageManager::BroadcastMessage
========================================================

.. csv-table::
	
	"**void BroadcastMessage(MessageMember* sender, MessageType type, void* data) const;**"

Send message to everyone on the list, except the sender. This is called when a member wants to send a message. The message is sent through :doc:`../../MessageMember` interface via OnMessage virtual method.

Parameters
----------

- **sender**
	Sender of the message

- **type**
	Message type

- **data**
	Additional data

See also
--------

- :doc:`../MessageManager`

- :doc:`../../MessageMember`

- :doc:`../../MessageTypes`
