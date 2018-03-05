:orphan:

Koala::Editor::Main::(operator)
===============================

.. csv-table::
	
	"**Main& operator=(const Main& other) = delete;**", "*(1) Copy*"
	"**Main& operator=(Main&& other) = delete;**", "*(2) Move*"

*(1) Copy assignment & (2) Move assignment*
	Copy and move operators are deleted, because this class is designed to be singular.

See also
--------

- :doc:`../Main`
