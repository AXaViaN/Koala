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

	constexpr float ButtonHeight = 0.1f;
	const Color ButtonColor = Color(0.75f, 0.75f, 0.75f);
	constexpr float ButtonSpacing = 0.05f;

	// Draw buttons
	if(Renderer::DrawIconButton(Utility::Icon::NewVariable, ButtonHeight, ButtonColor))
	{

	}
	Renderer::DrawSameLine(ButtonSpacing);
	if(Renderer::DrawIconButton(Utility::Icon::NewFunction, ButtonHeight, ButtonColor))
	{
		
	}

	// Draw trees
	Renderer::Spacing(3);
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
