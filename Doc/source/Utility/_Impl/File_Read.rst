:orphan:

Koala::Utility::File::Read
==========================

.. csv-table::
	
	"**std::string Read(size_t size);**", "*(1) Simple*"
	"**std::string ReadLine();**", "*(2) With Line*"

*(1) Simple*
	Read string from file.

*(2) With Line*
	Read one line from file.

----

Line ending is considered as line feed (:code:`\n`).

Parameters
----------

- **size**
	How many bytes to read

Return Value
------------

A string from file.

See also
--------

- :doc:`../File`
