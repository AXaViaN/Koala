#include <Koala/Editor/CodeBoard.h>
#include <Koala/Editor/Gfx/Renderer.h>

namespace Koala::Editor {

CodeBoard::CodeBoard(const Tool::Window& window) :
	Gfx::Panel(window, CodeBoardStartPosition(), CodeBoardEndPosition())
{
	m_FunctionList.emplace_back(Utility::Resource::GetText(Utility::Text::Program));
	m_SelectedFunction = 0;
}

void CodeBoard::OnGui()
{
	using namespace Gfx;

	// Draw buttons
	const size_t FunctionCount = m_FunctionList.size();
	const float ButtonSpacing = 0.001f;
	const float ButtonWidth = 1.0f/FunctionCount - ButtonSpacing;
	Vector2 cursorPosition;
	for( size_t i=0 ; i<FunctionCount ; ++i )
	{
		if(Renderer::DrawButton(m_FunctionList[i], {ButtonWidth, 0.04f}, m_SelectedFunction==i))
		{
			m_SelectedFunction = i;
		}
		cursorPosition = Renderer::GetCursorPosition();
		Renderer::DrawSameLine(ButtonSpacing);
	}

	// Button logic
	Renderer::SetCursorPosition(cursorPosition);
	Renderer::DrawText(m_FunctionList[m_SelectedFunction] + " code here.");
}

} // namespace Koala::Editor
