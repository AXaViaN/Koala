#include <Koala/Editor/CodeBoard.h>
#include <Koala/Editor/Gfx/Renderer.h>

namespace Koala::Editor {

static void SetupDefaultFunctions(Utility::Core::Node& programNode);

CodeBoard::CodeBoard(const Tool::Window& window) :
	Gfx::Panel(window, CodeBoardStartPosition(), CodeBoardEndPosition())
{
	m_FunctionList.emplace_back();
	m_FunctionList.back().first = Utility::Resource::GetText(Utility::Text::Program);
	m_FunctionList.back().second.emplace_back();
	m_FunctionList.back().second.back().Position = Gfx::Vector2(0.05f, 0.1f);

	SetupDefaultFunctions(m_FunctionList.back().second.back().Node);

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
		if(Renderer::DrawButton(m_FunctionList[i].first, {ButtonWidth, 0.04f}, m_SelectedFunction==i))
		{
			m_SelectedFunction = i;
		}
		cursorPosition = Renderer::GetCursorPosition();
		Renderer::DrawSameLine(ButtonSpacing);
	}

	// Draw Nodes
	Renderer::SetCursorPosition(cursorPosition);
	for( auto& sceneNode : m_FunctionList[m_SelectedFunction].second )
	{
		Renderer::DrawNode(sceneNode.Node, sceneNode.Position);
	}
}

static void SetupDefaultFunctions(Utility::Core::Node& programNode)
{
	// Program node
	{
		Utility::Core::FunctionInfo functionInfo;
		functionInfo.NameText = Utility::Text::Program;
		functionInfo.FrontSlots.emplace_back("1", Utility::Core::VariableType::None);
		functionInfo.FrontSlots.emplace_back("2", Utility::Core::VariableType::None);
		functionInfo.BackSlots.emplace_back("3", Utility::Core::VariableType::None);
		functionInfo.FrontSlots.emplace_back("4", Utility::Core::VariableType::None);
		functionInfo.BackSlots.emplace_back("5", Utility::Core::VariableType::None);
		functionInfo.FrontSlots.emplace_back("6", Utility::Core::VariableType::None);
		functionInfo.FrontSlots.emplace_back("7", Utility::Core::VariableType::None);
		functionInfo.BackSlots.emplace_back("8", Utility::Core::VariableType::None);
		functionInfo.FrontSlots.emplace_back("9", Utility::Core::VariableType::None);
		functionInfo.FrontSlots.emplace_back("10", Utility::Core::VariableType::None);
		functionInfo.BackSlots.emplace_back("11", Utility::Core::VariableType::None);
		functionInfo.FrontSlots.emplace_back("12", Utility::Core::VariableType::None);

		size_t programFunctionID = Utility::Core::FunctionManager::Add(functionInfo);
		programNode = Utility::Core::Node(programFunctionID);
	}
}

} // namespace Koala::Editor
