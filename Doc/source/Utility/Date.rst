Date
====

.. function:: class Koala::Utility::Date;

A time point in "day.month.year" format.

Public Methods
--------------

.. csv-table::
	
	":doc:`(constructor) <_Impl/Date_Ctor>`", "Constructors"
	":doc:`(operator) <_Impl/Date_Operator>`", "String operator"

.. csv-table::
	
	"Now", "Current date"
	"Setters", "Set methods for private members, clamps the values"
	"Getters", "Get methods for private members"
	"ToString", "Calls the string operator"

Private Members
---------------

.. csv-table::
	
	"m_Day", "Day [1, 31]"
	"m_Month", "Month [1, 12]"
	"m_Year", "Year [0, inf]"
