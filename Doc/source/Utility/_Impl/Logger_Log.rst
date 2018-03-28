:orphan:

Koala::Utility::Logger::Log
===========================

.. csv-table::
	
	"**void Log(const std::string& function, size_t line, LogType type, const std::string& message);**"

Log the message to the log file. The format is "Type: Message {function::line} [time]".

Parameters
----------

- **function**
	Function that called for logging

- **line**
	Line of the function

- **type**
	Logging type

- **message**
	Message to log

See also
--------

- :doc:`../Logger`
