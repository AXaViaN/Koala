Clock
=====

.. function:: class Koala::Utility::Clock;

A time point in "hour:minute:second" format.

Public Methods
--------------

.. csv-table::
	
	":doc:`(constructor) <_Impl/Clock_Ctor>`", "Constructors"
	":doc:`(operator) <_Impl/Clock_Operator>`", "String operator"

.. csv-table::
	
	"Now", "Current time in local timezone"
	"ProgramClock", "Time since the beginning"
	"Setters", "Set methods for private members, clamps the values"
	"Getters", "Get methods for private members"
	"ToString", "Calls the string operator"

Private Members
---------------

.. csv-table::
	
	"m_Hour", "Hour [0, inf]"
	"m_Minute", "Minute [0, 59]"
	"m_Second", "Second [0, 59]"
