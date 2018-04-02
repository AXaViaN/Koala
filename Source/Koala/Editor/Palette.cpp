#include <Koala/Editor/Palette.h>
#include <Koala/Editor/Gfx/Renderer.h>

namespace Koala::Editor {

Palette::Palette(const Tool::Window& window) :
	Gfx::Panel(window, 
			   Gfx::Vector2(CodeBoardEndPosition().GetX(), CodeBoardStartPosition().GetY()), 
			   Gfx::Vector2(1.0f, 1.0f))
{ }

void Palette::OnGui()
{
	Gfx::Renderer::DrawText("Palette");
}

} // namespace Koala::Editor
