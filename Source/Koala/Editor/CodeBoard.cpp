#include <Koala/Editor/CodeBoard.h>
#include <Koala/Editor/Gfx/Renderer.h>

namespace Koala::Editor {

CodeBoard::CodeBoard(const Tool::Window& window) :
	Gfx::Panel(window, CodeBoardStartPosition(), CodeBoardEndPosition())
{ }

void CodeBoard::OnGui()
{
	using namespace Gfx;

	static bool selection = true;

	if(Renderer::DrawButton(Utility::Text::Program, {0.5f, 0.04f}, selection))
	{
		selection = true;
	}
	Renderer::DrawSameLine();
	if(Renderer::DrawButton("Faktoriyel", {0.5f, 0.04f}, selection==false))
	{
		selection = false;
	}

	if(selection)
	{
		Renderer::SetCursorPosition({0.425f, 0.5f});
		Renderer::DrawText("Main program kodu");
	}
	else
	{
		Renderer::SetCursorPosition({0.4f, 0.5f});
		Renderer::DrawText("Faktoriyel fonksiyon kodu");
	}
}

} // namespace Koala::Editor
