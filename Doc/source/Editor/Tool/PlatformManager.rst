PlatformManager
===============

.. function:: class Koala::Editor::Tool::PlatformManager;

PlatformManager class is an abstraction for external libraries. It is expected to be initialized before everything else, and terminated on program exit.

Public Methods
--------------

.. csv-table::
	
	":doc:`Initialize <_Impl/PlatformManager_Initialize>`", "Initialize libraries"
	":doc:`InitializeRenderContext <_Impl/PlatformManager_InitializeRenderContext>`", "Create context for active window"
	":doc:`Terminate <_Impl/PlatformManager_Terminate>`", "Terminate libraries"
