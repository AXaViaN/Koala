:orphan:

Koala::Editor::Service::MessageMember::(constructor)
====================================================

.. csv-table::
	
	"**MessageMember();**", "*(1) Default*"
	"**MessageMember(const MessageMember& other);**", "*(2) Copy*"
	"**MessageMember(MessageMember&& other) noexcept;**", "*(3) Move*"

*(1) Default && (2) Copy && (3) Move*
	Register to message bus.

----

Constructor registers the object to the message bus via the static :doc:`../_Private/MessageManager`. Since this is an interface and there is no data, copy and move constructors doesn't do any extra work.

See also
--------

- :doc:`../MessageMember`

- :doc:`../_Private/MessageManager`
