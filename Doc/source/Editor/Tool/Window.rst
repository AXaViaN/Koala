Window
======

.. function:: class Koala::Editor::Tool::Window;

Window class is responsible for creating and managing windows.

Window handle is stored as void pointer. The real type is implementation detail and it is used by casting inside the source file.

Public Methods
--------------

.. csv-table::
	
	":doc:`(constructor) <_Impl/Window_Ctor>`", "Constructors"
	":doc:`(destructor) <_Impl/Window_Dtor>`", "Destructor"
	":doc:`(operator) <_Impl/Window_Operator>`", "Copy, move operators"

.. csv-table::
	
	":doc:`Create <_Impl/Window_Create>`", "Create window"
	":doc:`Destroy <_Impl/Window_Destroy>`", "Destroy window"
	":doc:`Activate <_Impl/Window_Activate>`", "Select window as render target"
	":doc:`Update <_Impl/Window_Update>`", "Swap buffers and update validation"
	":doc:`RenameTitle <_Impl/Window_RenameTitle>`", "Rename window title"
	":doc:`GetWidth <_Impl/Window_GetWidth>`", "Get window width"
	":doc:`GetHeight <_Impl/Window_GetHeight>`", "Get window height"
	":doc:`IsValid <_Impl/Window_IsValid>`", "Check validation"

Private Members
---------------

.. csv-table::
	
	"m_Handle", "Pointer to window handle"
