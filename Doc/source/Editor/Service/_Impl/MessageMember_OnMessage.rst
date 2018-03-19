:orphan:

Koala::Editor::Service::MessageMember::OnMessage
================================================

.. csv-table::
	
	"**virtual void OnMessage(MessageType type, void* data) = 0;**"

Message reciever. You can process the messages you want by checking the type.

Some messages are sent with additional data. A void pointer to the data is provided with the message. You need to cast it to the appropriate data structure. All the message types and data structures can be found on :doc:`../MessageTypes`.

Parameters
----------

- **type**
	Message type

- **data**
	Additional data

Example
-------

	.. code-block:: c++
		
		virtual void OnMessage(MessageType type, void* data) override
		{
		    if(type == MessageType::ProgramExit)
		    {
		        SaveConfig();
		    }
		}

See also
--------

- :doc:`../MessageMember`

- :doc:`../MessageTypes`
