#include <Koala/Editor/Palette.h>
#include <Koala/Editor/Gfx/Renderer.h>

namespace Koala::Editor {

Palette::Palette(const Tool::Window& window) :
	Gfx::Panel(window, 
			   Gfx::Vector2(CodeBoardEndPosition().GetX(), CodeBoardStartPosition().GetY()), 
			   Gfx::Vector2(1.0f, 1.0f))
{
	m_FunctionList[Utility::Text::Arithmetic] = {
		Utility::Text::Add,
		Utility::Text::Subtract,
		Utility::Text::Multiply,
		Utility::Text::Divide,
		Utility::Text::SquareRoot,
		Utility::Text::Power,
		Utility::Text::Modulo,
	};
	m_FunctionList[Utility::Text::System] = {
		Utility::Text::Print,
		Utility::Text::ReadNumber,
		Utility::Text::ReadString,
	};
}

void Palette::OnGui()
{
	using namespace Gfx;

	Renderer::DrawButton(Utility::Text::Palette, {1.0f, 0.04f});
	Renderer::Spacing(2);

	for( auto& category : m_FunctionList )
	{
		if(Renderer::DrawTree(category.first))
		{
			for( auto& function : category.second )
			{
				Renderer::DrawText(function);
			}

			Renderer::EndTree();
		}
	}
}

} // namespace Koala::Editor
