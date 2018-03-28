Logger
======

.. function:: class Koala::Utility::Logger;

This header also has an enumeration: :doc:`LogType <_Impl/Logger_LogType>`.

----

The logger can initialize itself statically. You can just use the macro functions.

On initialization, the logger will open a file on :code:`LogFilePath`. It will write the project name (if provided), current date and time. You can provide a project name by calling the :code:`Initialize` method before any calls to the logger.

If the log file fails to open, no logging will be performed.

The log file uses Windows line endings.

Preprocessor Macros
-------------------

.. csv-table::
	
	"LogInfo", "Log debug info to the user log file."
	"LogError", "Log error to the user log file."

Public Methods
--------------

.. csv-table::
	
	"Initialize", "Initialize the logger with a name tag"
	"Instance", "Singleton instance"
	":doc:`Log <_Impl/Logger_Log>`", "Log the message to the user log file"

Private Methods
---------------

.. csv-table::
	
	"(constructor)", "Default constructor to initialize the log file"

.. csv-table::
	
	"ProjectName", "This is just a wrapper to create a static member"

Private Members
---------------

.. csv-table::
	
	"LogFilePath", "Constant string for log file path"
	"m_LogFile", "The log file"
