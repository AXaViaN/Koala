:orphan:

Koala::Editor::Service::MessageMember::OnInput
==============================================

.. csv-table::
	
	"**virtual void OnInput(InputMessageType type, const InputMessageData& data) = 0;**"

Input reciever. Input handling is done from this interface.

You can get additional information about the input by data parameter. You can check the key type, the window that the input occured, etc. You can get more information about the input types and input data from :doc:`../MessageTypes`.

Parameters
----------

- **type**
	Input type

- **data**
	Additional data

Example
-------

	.. code-block:: c++
		
		virtual void OnInput(InputMessageType type, const InputMessageData& data) override
		{
		    if(data.Window == m_MainWindow && 
		       type == InputMessageType::KeyPress)
		    {
		        m_CurrentKey = data.Key;
		    }
		}

See also
--------

- :doc:`../MessageMember`

- :doc:`../MessageTypes`

- :doc:`../../Tool/Input`
