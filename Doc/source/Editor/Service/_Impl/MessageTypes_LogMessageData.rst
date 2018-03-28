:orphan:

LogMessageData
==============

.. function:: struct Koala::Editor::Service::LogMessageData;

Struct for log data. Logger needs the calling function and its line. To automate this process, there is a macro: :code:`GenerateLogMessageData()`.

Public Members
--------------

.. csv-table::
	
	"Message", "Message to log"
	"Function", "Function that called for logging"
	"Line", "Line of the function "

Example
-------

	.. code-block:: c++
		
		if(file.IsValid() == false)
		{
		    auto data = GenerateLogMessageData();
		    data.Message = "File is not available";
		    
		    SendMessage(MessageType::LogError, &data);
		}

See also
--------

- :doc:`../MessageTypes`
