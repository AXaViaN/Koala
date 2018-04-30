#include <Koala/Editor/CodeBoard.h>
#include <Koala/Editor/Private/CodeBoardButtonPanel.h>
#include <Koala/Editor/Gfx/Renderer.h>
#include <Koala/Editor/Tool/Input.h>

namespace Koala::Editor {

static void SetupDefaultFunctions(Utility::Core::Node& programNode);

CodeBoard::CodeBoard(const Tool::Window& window) :
	Gfx::Panel(window, 
			   Gfx::Vector2(CodeBoardStartPosition().GetX(), 
							CodeBoardStartPosition().GetY()+CodeBoardButtonPanel::ButtonHeight), 
			   CodeBoardEndPosition()),

	m_Window(window)
{
	Panel::DisableScrollBar();
	Panel::DisablePanelInput();

	m_FunctionList.emplace_back();
	m_FunctionList.back().Name = Utility::Resource::GetText(Utility::Text::Program);
	m_FunctionList.back().SceneNodes.emplace_back();
	m_FunctionList.back().SceneNodes.back().Position = Gfx::Vector2(0.05f, 0.1f);

#if 1
	m_FunctionList.back().SceneNodes.emplace_back();
	m_FunctionList.back().SceneNodes.back().Position = Gfx::Vector2(0.5f, 0.5f);
	{
		Utility::Core::FunctionInfo functionInfo;
		functionInfo.Name = "Topla";
		functionInfo.BackSlots.emplace_back("", Utility::Core::VariableType::None);
		functionInfo.BackSlots.emplace_back("X", Utility::Core::VariableType::Float64);
		functionInfo.BackSlots.emplace_back("Y", Utility::Core::VariableType::Float64);
		functionInfo.FrontSlots.emplace_back("", Utility::Core::VariableType::None);
		functionInfo.FrontSlots.emplace_back("Sonuc", Utility::Core::VariableType::Float64);

		size_t programFunctionID = Utility::Core::FunctionManager::Add(functionInfo);
		m_FunctionList.back().SceneNodes.back().Node = Utility::Core::Node(programFunctionID);
	}

	m_FunctionList.emplace_back();
	m_FunctionList.back().Name = "Test";
	m_FunctionList.back().SceneNodes.emplace_back();
	m_FunctionList.back().SceneNodes.back().Position = Gfx::Vector2(0.2f, 0.7f);
	{
		Utility::Core::FunctionInfo functionInfo;
		functionInfo.Name = "Yazdir";
		functionInfo.BackSlots.emplace_back("", Utility::Core::VariableType::None);
		functionInfo.BackSlots.emplace_back("", Utility::Core::VariableType::String);
		functionInfo.FrontSlots.emplace_back("", Utility::Core::VariableType::None);

		size_t programFunctionID = Utility::Core::FunctionManager::Add(functionInfo);
		m_FunctionList.back().SceneNodes.back().Node = Utility::Core::Node(programFunctionID);
	}
#endif

	SetupDefaultFunctions(m_FunctionList[0].SceneNodes[0].Node);

	m_SelectedFunction = 0;
}

void CodeBoard::OnGui()
{
	using namespace Gfx;

	// Draw buttons
	{
		std::vector<std::string> functionNames;
		for( size_t i=0 ; i<m_FunctionList.size() ; ++i )
		{
			functionNames.emplace_back(m_FunctionList[i].Name);
		}

		CodeBoardButtonPanel::Instance(m_Window).Update(functionNames, m_SelectedFunction);
	}

	// Draw Nodes
	for( auto& sceneNode : m_FunctionList[m_SelectedFunction].SceneNodes )
	{
		Renderer::DrawNode(sceneNode.Node, sceneNode.Position + m_FunctionList[m_SelectedFunction].DragOffset);
	}
}
void CodeBoard::OnInput(Service::InputMessageType type, const Service::InputMessageData& data)
{
	static Gfx::Vector2 oldMousePosition;

	switch(type)
	{
		case Koala::Editor::Service::InputMessageType::MousePress:
		{
			if(data.Button == Tool::MouseButtonType::Left)
			{
				oldMousePosition = Tool::Input::GetMousePosition();
			}
			break;
		}
		case Koala::Editor::Service::InputMessageType::MouseHold:
		{
			if(data.Button == Tool::MouseButtonType::Left)
			{
				auto mouseDelta = Tool::Input::GetMousePosition() - oldMousePosition;
				oldMousePosition = Tool::Input::GetMousePosition();

				m_FunctionList[m_SelectedFunction].DragOffset += mouseDelta;
			}
			break;
		}
	}
}

static void SetupDefaultFunctions(Utility::Core::Node& programNode)
{
	// Program node
	{
		Utility::Core::FunctionInfo functionInfo;
		functionInfo.NameText = Utility::Text::Program;
		functionInfo.FrontSlots.emplace_back("", Utility::Core::VariableType::None);

		size_t programFunctionID = Utility::Core::FunctionManager::Add(functionInfo);
		programNode = Utility::Core::Node(programFunctionID);
	}
}

} // namespace Koala::Editor
