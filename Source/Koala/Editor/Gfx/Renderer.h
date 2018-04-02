#ifndef KOALA__EDITOR__GFX__RENDERER
#define KOALA__EDITOR__GFX__RENDERER

#include <Koala/Editor/Gfx/Color.h>
#include <string>

namespace Koala::Editor::Gfx {

class Renderer
{
public:
	static void Initialize();
	static void Terminate();

	static void ClearViewport(const Color& color);
	static void Update();

	static void DrawGuiDemo();
	static void DrawText(const std::string& text);

};

} // namespace Koala::Editor::Gfx

#endif // KOALA__EDITOR__GFX__RENDERER
