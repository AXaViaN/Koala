:orphan:

Koala::Utility::File::(constructor)
===================================

.. csv-table::
	
	"**File(const std::string& path);**", "*(1) Initialize*"
	"**File(const File& other) = delete;**", "*(2) Copy*"
	"**File(File&& other) noexcept;**", "*(3) Move*"

*(1) Initialize constructor*
	Open the file.

*(2) Copy constructor*
	Copy constructor is deleted. Only one object can use a file.

*(3) Move constructor*
	Moves the file handle to this object.

----

File constructor opens the file. There is no other way to open and use a file. If the folders on the path are not available, they are automatically created.

You can check the succes via :doc:`IsValid <File_IsValid>` method.

Parameters
----------

- **path**
	Path to file

See also
--------

- :doc:`../File`

- :doc:`IsValid <File_IsValid>`
