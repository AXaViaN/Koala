File
====

.. function:: class Koala::Utility::File;

File class is a wrapper for file I/O.

File handle is stored as void pointer. The real type is :code:`std::FILE*` but it is hidden, we don't want to expose low level I/O to rest of the code.

Every file opens in binary mode and the read/write head is positioned at the end of file.

Public Methods
--------------

.. csv-table::
	
	":doc:`(constructor) <_Impl/File_Ctor>`", "Constructors"
	":doc:`(destructor) <_Impl/File_Dtor>`", "Destructor"
	":doc:`(operator) <_Impl/File_Operator>`", "Copy, move operators"

.. csv-table::
	
	":doc:`Write <_Impl/File_Write>`", "Write to file"
	":doc:`WriteLine <_Impl/File_Write>`", "Write to file with line feed"
	":doc:`Read <_Impl/File_Read>`", "Read from file"
	":doc:`ReadLine <_Impl/File_Read>`", "Read from file until a line feed"
	":doc:`MoveHead <_Impl/File_MoveHead>`", "Move file head position"
	":doc:`MoveHeadToFront <_Impl/File_MoveHead>`", "Move file head to the beginning"
	":doc:`MoveHeadToEnd <_Impl/File_MoveHead>`", "Move file head to the end"
	":doc:`Delete <_Impl/File_Delete>`", "Delete file from disk"
	":doc:`DeleteContents <_Impl/File_DeleteContents>`", "Delete file contents"
	":doc:`IsValid <_Impl/File_IsValid>`", "Check validation"

Private Members
---------------

.. csv-table::
	
	"m_Handle", "Pointer to file handle"
	"m_Path", "Path to file"
