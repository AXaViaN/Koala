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
	using namespace Gfx;

	Renderer::DrawButton(Utility::Text::ControlPanel, {1.0f, 0.08f});
	Renderer::Spacing(2);

	if(Renderer::DrawTree(Utility::Text::Functions))
	{
		
		Renderer::EndTree();
	}
	if(Renderer::DrawTree(Utility::Text::Variables))
	{

		Renderer::EndTree();
	}
}

} // namespace Koala::Editor
