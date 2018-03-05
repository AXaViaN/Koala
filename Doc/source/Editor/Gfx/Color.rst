Color
=====

.. function:: struct Koala::Editor::Gfx::Color;

Color class represents 4 component RGBA color-space. Every color value is stored as float variables. Default range is [0, 1] but it can be used with any range.

Raw values are stored in a float array. It makes internal operations easier by allowing iteration. Public members are just references to this array.

Public Methods
--------------

.. csv-table::
	
	":doc:`(constructor) <_Impl/Color_Ctor>`", "Constructors"

.. csv-table::
	
	":doc:`Clamp <_Impl/Color_Clamp>`", "Clamp colors to a range"

Public Members
--------------

.. csv-table::
	
	"r", "Red color component"
	"g", "Green color component"
	"b", "Blue color component"
	"a", "Alpha transparency component"

Private Members
---------------

.. csv-table::
	
	"m_RGBA", "4 component raw color array"
