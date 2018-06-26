#include <Koala/Editor/ControlPanel.h>
#include <Koala/Editor/Gfx/Renderer.h>

namespace Koala::Editor {

ControlPanel::ControlPanel(const Tool::Window& window) : 
	Gfx::Panel(window, 
			   Gfx::Vector2(0.0f, CodeBoardStartPosition().GetY()), 
			   Gfx::Vector2(CodeBoardStartPosition().GetX(), 0.5f+CodeBoardStartPosition().GetY()/2))
{
	m_VariableFunctionIDs.emplace_back();
}

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
		Utility::Core::FunctionInfo variableFunctionInfo;
		variableFunctionInfo.BackSlots.emplace_back("", Utility::Core::VariableType::None);
		variableFunctionInfo.BackSlots.emplace_back(Utility::Text::Condition, Utility::Core::VariableType::Boolean);
		variableFunctionInfo.BackSlots.emplace_back("", Utility::Core::VariableType::Float64);
		variableFunctionInfo.FrontSlots.emplace_back("", Utility::Core::VariableType::None);
		variableFunctionInfo.FrontSlots.emplace_back("", Utility::Core::VariableType::Float64);

		auto newFunctionID = Utility::Core::FunctionManager::Add(variableFunctionInfo);
		m_VariableFunctionIDs[m_SelectedFunction].emplace_back(newFunctionID);

		// Give the NameText on edit to prevent it filling Name
		variableFunctionInfo.ID = newFunctionID;
		variableFunctionInfo.NameText = Utility::Text::Variable;
		Utility::Core::FunctionManager::Edit(newFunctionID, variableFunctionInfo);

		Service::EditVariableFunctionData data = {newFunctionID};
		SendMessage(Service::MessageType::EditVariableFunction, &data);
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
		for( auto& functionID : m_VariableFunctionIDs[m_SelectedFunction] )
		{
			auto& functionName = Utility::Core::FunctionManager::Get(functionID).Name;
			if(functionName.size() > 0 && 
			   functionName[0] != '\0' && 
			   Renderer::DrawText(functionName))
			{
				Service::EditVariableFunctionData data = {functionID};
				SendMessage(Service::MessageType::EditVariableFunction, &data);
			}
		}

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
			m_VariableFunctionIDs = {};
			m_SelectedFunction = 0;

			m_VariableFunctionIDs.emplace_back();

			break;
		}
		case Service::MessageType::SaveProject:
		{
			auto saveData = static_cast<Service::SaveProjectData*>(data);

			saveData->UserFunctionIDs = m_UserFunctionIDs;
			saveData->VariableFunctionIDs = m_VariableFunctionIDs;

			break;
		}
		case Service::MessageType::LoadProject:
		{
			auto loadData = static_cast<Service::LoadProjectData*>(data);

			m_UserFunctionIDs = loadData->UserFunctionIDs;
			m_VariableFunctionIDs = loadData->VariableFunctionIDs;
			m_SelectedFunction = loadData->SelectedFunction;

			// Fix variable names
			for( auto& variableFunctionIDs : m_VariableFunctionIDs )
			{
				for( auto& variableFunctionID : variableFunctionIDs )
				{
					auto variableFunctionInfo = Utility::Core::FunctionManager::Get(variableFunctionID);
					variableFunctionInfo.Name = variableFunctionInfo.FrontSlots.back().GetName();

					Utility::Core::FunctionManager::Edit(variableFunctionID, variableFunctionInfo);
				}
			}

			break;
		}
		case Service::MessageType::RemoveUserFunction:
		{
			auto userFunctionData = static_cast<Service::RemoveUserFunctionData*>(data);

			for( size_t i=0 ; i<m_UserFunctionIDs.size() ; ++i )
			{
				if(m_UserFunctionIDs[i] == userFunctionData->FunctionID)
				{
					Utility::Core::FunctionManager::Remove(m_UserFunctionIDs[i]);
					m_UserFunctionIDs.erase(m_UserFunctionIDs.begin() + i);

					for( auto& variableFunctionID : m_VariableFunctionIDs[i+1] )
					{
						Utility::Core::FunctionManager::Remove(variableFunctionID);
					}
					m_VariableFunctionIDs.erase(m_VariableFunctionIDs.begin() + i+1);

					if(i == m_SelectedFunction)
					{
						m_SelectedFunction = 0;
					}

					break;
				}
			}

			break;
		}
		case Service::MessageType::RemoveVariableFunction:
		{
			auto variableFunctionData = static_cast<Service::RemoveVariableFunctionData*>(data);

			for( auto& it=m_VariableFunctionIDs[m_SelectedFunction].begin() ; it!=m_VariableFunctionIDs[m_SelectedFunction].end() ; ++it )
			{
				auto& variableFunctionID = *it;

				if(variableFunctionID == variableFunctionData->FunctionID)
				{
					Utility::Core::FunctionManager::Remove(variableFunctionID);
					m_VariableFunctionIDs[m_SelectedFunction].erase(it);
					break;
				}
			}

			break;
		}
		case Service::MessageType::FunctionSelected:
		{
			auto functionSelectedData = static_cast<Service::FunctionSelectedData*>(data);

			m_SelectedFunction = functionSelectedData->FunctionIndex;

			while(m_SelectedFunction+1 > m_VariableFunctionIDs.size())
			{
				m_VariableFunctionIDs.emplace_back();
			}

			break;
		}
	}
}

} // namespace Koala::Editor
