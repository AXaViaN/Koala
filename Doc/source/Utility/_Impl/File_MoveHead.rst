:orphan:

Koala::Utility::File::MoveHead
==============================

.. csv-table::
	
	"**void MoveHead(int delta);**", "*(1) Simple*"
	"**void MoveHeadToFront();**", "*(2) Front*"
	"**void MoveHeadToEnd();**", "*(3) End*"

*(1) Simple*
	Move the read/write head by delta.

*(2) Front*
	Move the read/write head to the beginning.

*(3) End*
	Move the read/write head to the end.

Parameters
----------

- **delta**
	Distance from current position, in bytes.

See also
--------

- :doc:`../File`
