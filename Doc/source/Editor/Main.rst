Main
====

.. function:: class Koala::Editor::Main final;

Core class of the editor. It is designed to abstract everything from main function.

Public Methods
--------------

.. csv-table::
	
	":doc:`(constructor) <_Impl/Main_Ctor>`", "Default, copy, move constructors"
	":doc:`(destructor) <_Impl/Main_Dtor>`", "Destructor"
	":doc:`(operator) <_Impl/Main_Operator>`", "Copy, move operators"

.. csv-table::
	
	":doc:`Run <_Impl/Main_Run>`", "Main program loop"

Private Members
---------------

.. csv-table::
	
	"m_MainWindow", "Editor window"
	"m_TestWindow", "Testing multiple windows"
	"m_CanRun", "Boolean to check initialization succes"
