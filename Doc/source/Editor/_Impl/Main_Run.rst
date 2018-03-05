:orphan:

Koala::Editor::Main::Run
========================

.. csv-table::
	
	"**void Run();**"

Run method has the main program loop. It runs until the main window is closed. It is safe to call even if the initialization fails on constructor, it checks the initialization success.

Example
-------

	.. code-block:: c++
		
		#include <Koala/Editor/Main.h>
		
		int main()
		{
		    Koala::Editor::Main program;
		    
		    program.Run();
		    
		    return 0;
		}

See also
--------

- :doc:`../Main`
