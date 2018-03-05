:orphan:

Koala::Editor::Gfx::Color::Clamp
================================

.. csv-table::
	
	"**void Clamp();**", "*(1) Default*"
	"**void Clamp(float range0, float range1);**", "*(2) Ranged*"

*(1) Default*
	Calls *(2) Ranged* version with default range of [0, 1].

*(2) Ranged*
	Clamps all components (including alpha) to given range. Order of the range values are not important.

Parameters
----------

- **range0**
	Range start, inclusive

- **range1**
	Range end, inclusive

Example
-------

	.. code-block:: c++
		
		#include <Koala/Editor/Gfx/Color.h>
		
		int main()
		{
		    using namespace Koala::Editor::Gfx;
		    
		    Color color;
		    std::printf("%f, %f, %f, %f\n", 
		                color.r, color.g, color.b, color.a);
		    
		    color.r = 0.7f;
		    color.g = 3.0f;
		    color.Clamp(0.5f, 2.0f);
		    std::printf("%f, %f, %f, %f\n", 
		                color.r, color.g, color.b, color.a);
		    
		    color.Clamp();
		    std::printf("%f, %f, %f, %f\n", 
		                color.r, color.g, color.b, color.a);
		    
		    return 0;
		}

**Output:**
	::
	
		0.0, 0.0, 0.0, 1.0
		0.7, 2.0, 0.5, 1.0
		0.7, 1.0, 0.5, 1.0

See also
--------

- :doc:`../Color`
