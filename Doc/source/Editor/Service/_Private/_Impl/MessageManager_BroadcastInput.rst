:orphan:

Koala::Editor::Service::MessageManager::BroadcastInput
======================================================

.. csv-table::
	
	"**void BroadcastInput(MessageMember* sender, InputMessageType type, const Service::InputMessageData& data) const;**"

Send input to everyone on the list, except the sender. This is usually called from :doc:`../../../Tool/Input` when a user input is recieved. The input is sent through :doc:`../../MessageMember` interface via OnInput virtual method.

Parameters
----------

- **sender**
	Sender of the input

- **type**
	Input type

- **data**
	Additional data

See also
--------

- :doc:`../MessageManager`

- :doc:`../../MessageMember`

- :doc:`../../MessageTypes`

- :doc:`../../../Tool/Input`
