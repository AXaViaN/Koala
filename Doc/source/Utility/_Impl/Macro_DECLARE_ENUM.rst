:orphan:

DECLARE_ENUM
============

.. csv-table::
	
	"**DECLARE_ENUM(name, values)**"

Creates an enumeration with string values. You need to define a list of values and optional assignments before calling this macro. Examine the example for the usage.

It has some other macro functions to accomplish this task; :code:`VALUE_TO_ENUM`, :code:`VALUE_TO_ENUM_ASSIGNMENT`, :code:`VALUE_TO_STRING`.

Parameters
----------

- **name**
	Name of the enum

- **values**
	List of values for enumeration

Example
-------

	.. code-block:: c++
		
		#define MESSAGE_TYPES(value, assignment) \
		    value(ProgramExit) \
		    assignment(LogInfo) \
		    value(LogError)
		
		#define ASSIGNMENT_LogInfo 8
		
		// This creates;
		// enum class MessageType; and
		// static const std::string& MessageTypeToString(MessageType value);
		DECLARE_ENUM(MessageType, MESSAGE_TYPES)
		
		void Print(MessageType type)
		{
		    std::printf("%s = %d\n", MessageTypeToString(type).c_str(),
		                int(type));
		}
		
		int main()
		{
		    Print(MessageType::ProgramExit);
		    Print(MessageType::LogInfo);
		    Print(MessageType::LogError);
		    
		    return 0;
		}

**Output:**
	::
	
		ProgramExit = 0
		LogInfo = 8
		LogError = 9

See also
--------

- :doc:`../Macro`
