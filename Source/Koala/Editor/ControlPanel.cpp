#include <Koala/Editor/ControlPanel.h>
#include <Koala/Editor/Gfx/Renderer.h>

namespace Koala::Editor {

ControlPanel::ControlPanel(const Tool::Window& window) : 
	Gfx::Panel(window, 
			   Gfx::Vector2(0.0f, CodeBoardStartPosition().GetY()), 
			   Gfx::Vector2(CodeBoardStartPosition().GetX(), 0.5f+CodeBoardStartPosition().GetY()/2))
{ }

void ControlPanel::OnGui()
{
	Gfx::Renderer::DrawText("Control Panel");
}

} // namespace Koala::Editor
