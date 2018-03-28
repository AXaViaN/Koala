:orphan:

KeyType
=======

.. function:: enum class Koala::Editor::Tool::KeyType;

Enumeration for keys. These key codes are inherited from the `GLFW: Keyboard keys <http://www.glfw.org/docs/latest/group__keys.html>`_.

This enum is created with macros that provides string versions of the values.

.. csv-table::
	
	"Unknown = -1"
	
	"Space				=  "
	"Apostrophe			= '"
	"Comma				= ,"
	"Minus				= -"
	"Period				= ."
	"Slash				= /"
	
	"Key0				= 0"
	"Key1"
	"Key2"
	"Key3"
	"Key4"
	"Key5"
	"Key6"
	"Key7"
	"Key8"
	"Key9"
	
	"Semicolon			= ;"
	"Equal				= ="
	
	"A					= A"
	"B"
	"C"
	"D"
	"E"
	"F"
	"G"
	"H"
	"I"
	"J"
	"K"
	"L"
	"M"
	"N"
	"O"
	"P"
	"Q"
	"R"
	"S"
	"T"
	"U"
	"V"
	"W"
	"X"
	"Y"
	"Z"
	
	"LeftBracket			= ["
	"BackSlash			= \\"
	"RightBracket		= ]"
	"GraveAccent			= `"
	
	"Escape				= 256"
	"Enter"
	"Tab"
	"Backspace"
	"Insert"
	"Delete"
	"Right"
	"Left"
	"Down"
	"Up"
	"PageUp"
	"PageDown"
	"Home"
	"End"
	
	"CapsLock			= 280"
	"ScrollLock"
	"NumLock"
	"PrintScreen"
	"Pause"
	
	"F1					= 290"
	"F2"
	"F3"
	"F4"
	"F5"
	"F6"
	"F7"
	"F8"
	"F9"
	"F10"
	"F11"
	"F12"
	"F13"
	"F14"
	"F15"
	"F16"
	"F17"
	"F18"
	"F19"
	"F20"
	"F21"
	"F22"
	"F23"
	"F24"
	"F25"
	
	"NumPad0				= 320"
	"NumPad1"
	"NumPad2"
	"NumPad3"
	"NumPad4"
	"NumPad5"
	"NumPad6"
	"NumPad7"
	"NumPad8"
	"NumPad9"
	"NumPadDecimal"
	"NumPadDivide"
	"NumPadMultiply"
	"NumPadSubtract"
	"NumPadAdd"
	"NumPadEnter"
	"NumPadEqual"
	
	"LeftShift			= 340"
	"LeftControl"
	"LeftAlt"
	"LeftSuper"
	"RightShift"
	"RightControl"
	"RightAlt"
	"RightSuper"
	"Menu"

Example
-------

	.. code-block:: c++
		
		void PrintKeyType(KeyType keyType)
		{
		    std::printf("%s\n", KeyTypeToString(keyType).c_str());
		}

See also
--------

- :doc:`../Input`
