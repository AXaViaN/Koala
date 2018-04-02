#include <Koala/Editor/ToolBar.h>
#include <Koala/Editor/Gfx/Renderer.h>

namespace Koala::Editor {

ToolBar::ToolBar(const Tool::Window& window) :
	Gfx::Panel(window, 
			   Gfx::Vector2(0.0f, 0.0f), 
			   Gfx::Vector2(1.0f, CodeBoardStartPosition().GetY()))
{ }

void ToolBar::OnGui()
{
	Gfx::Renderer::DrawText("Tool Bar");
}

} // namespace Koala::Editor
