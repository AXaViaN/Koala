#include <Koala/Editor/CodeBoard.h>
#include <Koala/Editor/Gfx/Renderer.h>

namespace Koala::Editor {

CodeBoard::CodeBoard(const Tool::Window& window) :
	Gfx::Panel(window, CodeBoardStartPosition(), CodeBoardEndPosition())
{ }

void CodeBoard::OnGui()
{
	Gfx::Renderer::DrawText("Code Board");
}

} // namespace Koala::Editor
