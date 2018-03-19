:orphan:

Koala::Editor::Service::MessageMember::SendInput
================================================

.. csv-table::
	
	"**void SendInput(InputMessageType type, const Service::InputMessageData& data);**"

Broadcast an input via the static :doc:`../_Private/MessageManager`.

This method is usually called by the :doc:`../../Tool/Input` class. You need to pass a valid data containing all the relevant information about the input type. More information about the input types and input data can be found on :doc:`../MessageTypes`.

Parameters
----------

- **type**
	Input type

- **data**
	Additional data

See also
--------

- :doc:`../MessageMember`

- :doc:`../_Private/MessageManager`

- :doc:`../MessageTypes`

- :doc:`../../Tool/Input`
