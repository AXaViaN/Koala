#ifndef PRIVATE__KOALA__EDITOR__CODE_BOARD_BUTTON_PANEL
#define PRIVATE__KOALA__EDITOR__CODE_BOARD_BUTTON_PANEL

#include <Koala/Editor/Gfx/Panel.h>
#include <Koala/Editor/Gfx/Renderer.h>
#include <vector>
#include <string>

namespace Koala::Editor {

class CodeBoardButtonPanel final : private Gfx::Panel
{
public:
	CodeBoardButtonPanel(const Tool::Window& window) : 
		Gfx::Panel(window, 
				   CodeBoardStartPosition(), 
				   Gfx::Vector2(CodeBoardEndPosition().GetX(), CodeBoardStartPosition().GetY()+ButtonHeight))
	{
		Panel::DisableScrollBar();
	}

	static CodeBoardButtonPanel& Instance(const Tool::Window& window)
	{
		static CodeBoardButtonPanel instance(window);
		return instance;
	}

	void Update(const std::vector<std::string>& functionList, size_t& selectedFunction)
	{
		m_FunctionList = functionList;
		m_SelectedFunction = selectedFunction;

		Panel::Update();

		selectedFunction = m_SelectedFunction;
	}

public:
	static constexpr float ButtonHeight = 0.04f;

private:
	virtual void OnGui() override
	{
		using namespace Gfx;

		// Draw buttons
		const size_t FunctionCount = m_FunctionList.size();
		const float ButtonSpacing = 0.001f;
		const float ButtonWidth = 1.0f/FunctionCount - ButtonSpacing;
		for( size_t i=0 ; i<FunctionCount ; ++i )
		{
			if(Renderer::DrawButton(m_FunctionList[i], {ButtonWidth, 1.0f}, m_SelectedFunction==i))
			{
				m_SelectedFunction = i;
			}
			Renderer::DrawSameLine(ButtonSpacing);
		}
	}

private:
	std::vector<std::string> m_FunctionList;
	size_t m_SelectedFunction;

};

} // namespace Koala::Editor

#endif // PRIVATE__KOALA__EDITOR__CODE_BOARD_BUTTON_PANEL
