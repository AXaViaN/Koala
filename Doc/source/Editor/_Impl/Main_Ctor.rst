:orphan:

Koala::Editor::Main::(constructor)
==================================

.. csv-table::
	
	"**Main();**", "*(1) Default*"
	"**Main(const Main& other) = delete;**", "*(2) Copy*"
	"**Main(Main&& other) = delete;**", "*(3) Move*"

*(1) Default constructor*
	Initializes everything needed in the program;
		
		- Initialize platform libraries via :doc:`../Tool/PlatformManager`
		- Create editor window

*(2) Copy constructor & (3) Move constructor*
	Copy and move constructors are deleted, because this class is designed to be singular.

See also
--------

- :doc:`../Main`

- :doc:`Main::(destructor) <Main_Dtor>`

- :doc:`Main::Run <Main_Run>`
