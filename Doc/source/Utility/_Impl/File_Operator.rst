:orphan:

Koala::Utility::File::(operator)
================================

.. csv-table::
	
	"**File& operator=(const File& other) = delete;**", "*(1) Copy*"
	"**File& operator=(File&& other) noexcept;**", "*(2) Move*"

*(1) Copy assignment*
	Copy operator is deleted. Only one object can use a file.

*(2) Move assignment*
	Moves the file handle to this object.

See also
--------

- :doc:`../File`
