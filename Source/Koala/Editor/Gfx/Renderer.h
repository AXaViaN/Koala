#ifndef KOALA__EDITOR__GFX__RENDERER
#define KOALA__EDITOR__GFX__RENDERER

#include <Koala/Editor/Gfx/Color.h>

namespace Koala::Editor::Gfx {

class Renderer
{
public:
	static void ClearViewport(const Color& color);

};

} // namespace Koala::Editor::Gfx

#endif // KOALA__EDITOR__GFX__RENDERER
