:orphan:

Koala::Utility::File::Write
===========================

.. csv-table::
	
	"**void Write(const std::string& data);**", "*(1) Simple*"
	"**void WriteLine(const std::string& data);**", "*(2) With Line*"

*(1) Simple*
	Write string data to file.

*(2) With Line*
	Write string data to file with line feed.

----

*(2) With Line* version internally calls *(1) Simple* version with the line feed.

Parameters
----------

- **data**
	Input string

See also
--------

- :doc:`../File`
