:orphan:

Koala::Editor::Service::MessageMember::SendMessage
==================================================

.. csv-table::
	
	"**void SendMessage(MessageType type, void* data);**"

Broadcast a message via the static :doc:`../_Private/MessageManager`.

Some messages are expected to be sent with additional data. You need to create data with the appropriate data structure and pass it. All the message types and data structures can be found on :doc:`../MessageTypes`.

Parameters
----------

- **type**
	Message type

- **data**
	Additional data. If there is no data, pass :code:`nullptr`

See also
--------

- :doc:`../MessageMember`

- :doc:`../_Private/MessageManager`

- :doc:`../MessageTypes`
