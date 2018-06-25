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
		Utility::Core::FunctionInfo newFunctionInfo;
		newFunctionInfo.BackSlots.emplace_back("", Utility::Core::VariableType::None);
		newFunctionInfo.FrontSlots.emplace_back("", Utility::Core::VariableType::None);

		auto newFunctionID = Utility::Core::FunctionManager::Add(newFunctionInfo);
		m_UserFunctionIDs.emplace_back(newFunctionID);

		Service::EditUserFunctionData data = {newFunctionID};
		SendMessage(Service::MessageType::EditUserFunction, &data);
	}

	// Draw trees
	Renderer::Spacing(3);
	if(Renderer::DrawTree(Utility::Text::Functions))
	{
		for( auto& functionID : m_UserFunctionIDs )
		{
			auto& functionName = Utility::Core::FunctionManager::Get(functionID).Name;
			if(functionName.size() > 0 && 
			   functionName[0] != '\0' && 
			   Renderer::DrawText(functionName))
			{
				Service::EditUserFunctionData data = {functionID};
				SendMessage(Service::MessageType::EditUserFunction, &data);
			}
		}
		
		Renderer::EndTree();
	}
	if(Renderer::DrawTree(Utility::Text::Variables))
	{

		Renderer::EndTree();
	}
}
void ControlPanel::OnMessage(Service::MessageType type, void* data)
{
	switch(type)
	{
		case Service::MessageType::NewProject:
		{
			m_UserFunctionIDs = {};
			break;
		}
		case Service::MessageType::RemoveUserFunction:
		{
			auto userFunctionData = static_cast<Service::RemoveUserFunctionData*>(data);

			for( auto& it=m_UserFunctionIDs.begin() ; it!=m_UserFunctionIDs.end() ; ++it )
			{
				auto& userFunctionID = *it;

				if(userFunctionID == userFunctionData->FunctionID)
				{
					Utility::Core::FunctionManager::Remove(userFunctionID);
					m_UserFunctionIDs.erase(it);
					break;
				}
			}

			break;
		}
	}
}

} // namespace Koala::Editor
