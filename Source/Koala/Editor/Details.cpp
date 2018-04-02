#include <Koala/Editor/Details.h>
#include <Koala/Editor/Gfx/Renderer.h>

namespace Koala::Editor {

Details::Details(const Tool::Window& window) :
	Gfx::Panel(window, 
			   Gfx::Vector2(0.0f, 0.5f+CodeBoardStartPosition().GetY()/2), 
			   Gfx::Vector2(CodeBoardStartPosition().GetX(), 1.0f))
{ }

void Details::OnGui()
{
	Gfx::Renderer::DrawText("Details");
}

} // namespace Koala::Editor
