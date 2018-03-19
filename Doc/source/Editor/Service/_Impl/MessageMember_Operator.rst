:orphan:

Koala::Editor::Service::MessageMember::(operator)
=================================================

.. csv-table::
	
	"**MessageMember& operator=(const MessageMember& other);**", "*(1) Copy*"
	"**MessageMember& operator=(MessageMember&& other) noexcept;**", "*(2) Move*"

*(1) Copy assignment && (2) Move assignment*
	Implemented to comply with the rule of five. Just returns this object.

See also
--------

- :doc:`../MessageMember`
