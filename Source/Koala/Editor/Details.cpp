#include <Koala/Editor/Details.h>
#include <Koala/Editor/Gfx/Renderer.h>

namespace Koala::Editor {

static std::string UserFunctionName(256u, '\0');
static std::vector<Utility::Core::Variable> UserFunctionInputs;
static std::vector<Utility::Core::Variable> UserFunctionOutputs;

Details::Details(const Tool::Window& window) :
	Gfx::Panel(window, 
			   Gfx::Vector2(0.0f, 0.5f+CodeBoardStartPosition().GetY()/2), 
			   Gfx::Vector2(CodeBoardStartPosition().GetX(), 1.0f))
{ }

void Details::OnGui()
{
	using namespace Gfx;

	const Vector2 BigButtonSize = Vector2(1.0f, 0.08f);
	const Vector2 SmallButtonSize = Vector2(0.75f, 0.05f);
	constexpr size_t SeperateSpacing = 3;

	Renderer::DrawButton(Utility::Text::Details, BigButtonSize);
	Renderer::Spacing(2);

	if(m_CurrentUserFunction != -1)
	{
		auto userFunction = Utility::Core::FunctionManager::Get(m_CurrentUserFunction);

		// Function edit
		Renderer::DrawText(Utility::Text::FunctionName);
		Renderer::InputText(&UserFunctionName);

		int count = 0;
		count = UserFunctionInputs.size();
		Renderer::DrawText(Utility::Text::InputCount);
		Renderer::PushItemID((int)Utility::Text::InputCount);
		Renderer::InputInt(&count);
		Renderer::PopItemID();
		if(count < 0)
		{
			count = 0;
		}
		UserFunctionInputs.resize(count, Utility::Core::Variable("", Utility::Core::VariableType::Float64));

		count = UserFunctionOutputs.size();
		Renderer::DrawText(Utility::Text::OutputCount);
		Renderer::PushItemID((int)Utility::Text::OutputCount);
		Renderer::InputInt(&count);
		Renderer::PopItemID();
		if(count < 0)
		{
			count = 0;
		}
		UserFunctionOutputs.resize(count, Utility::Core::Variable("", Utility::Core::VariableType::Float64));

		Renderer::Spacing(SeperateSpacing);
		Renderer::DrawSeperator();
		Renderer::Spacing(SeperateSpacing);

		// Function node spawner
		auto cursorPosition = Renderer::GetCursorPosition();
		cursorPosition.SetX((1.0f - SmallButtonSize.GetX()) / 2.0f);
		Renderer::SetCursorPosition(cursorPosition);
		if(Renderer::DrawButton(Utility::Text::Spawn, SmallButtonSize) && 
		   userFunction.Name.size() > 0 && 
		   userFunction.Name[0] != '\0')
		{
			Service::SpawnNodeData data = {m_CurrentUserFunction};
			SendMessage(Service::MessageType::SpawnNode, &data);
		}

		Renderer::Spacing(SeperateSpacing);
		Renderer::DrawSeperator();
		Renderer::Spacing(SeperateSpacing);

		// Input/Output helpers
		auto ioVariableEdit = [](std::vector<Utility::Core::Variable>& variables)
		{
			std::map<Utility::Core::VariableType, size_t> variableTypeIndices;
			variableTypeIndices[Utility::Core::VariableType::Float64] = 0;
			variableTypeIndices[Utility::Core::VariableType::String] = 1;
			variableTypeIndices[Utility::Core::VariableType::Boolean] = 2;
			std::map<size_t, Utility::Core::VariableType> variableTypeIndicesReverse;
			variableTypeIndicesReverse[0] = Utility::Core::VariableType::Float64;
			variableTypeIndicesReverse[1] = Utility::Core::VariableType::String;
			variableTypeIndicesReverse[2] = Utility::Core::VariableType::Boolean;
			const std::vector<std::string> comboElements = {
				Utility::Resource::GetText(Utility::Text::Number).c_str(),
				Utility::Resource::GetText(Utility::Text::String).c_str(),
				Utility::Resource::GetText(Utility::Text::Boolean).c_str(),
			};

			for( auto& variable : variables )
			{
				Renderer::PushItemID(&variable);
				{
					std::string name = variable.GetName();
					name.resize(256u, '\0');
					Renderer::InputText(&name);

					int typeSelection = variableTypeIndices[variable.GetVariableType()];
					Renderer::ComboBox(&typeSelection, comboElements);
					auto type = variableTypeIndicesReverse[typeSelection];

					variable = Utility::Core::Variable(name, type);
				}
				Renderer::PopItemID();
			}
		};

		Renderer::DrawText(Utility::Text::Inputs);
		ioVariableEdit(UserFunctionInputs);

		Renderer::Spacing(SeperateSpacing);
		Renderer::DrawSeperator();
		Renderer::Spacing(SeperateSpacing);

		Renderer::DrawText(Utility::Text::Outputs);
		ioVariableEdit(UserFunctionOutputs);

		Renderer::Spacing(SeperateSpacing);
		Renderer::DrawSeperator();
		Renderer::Spacing(SeperateSpacing);

		// Function apply/remove
		cursorPosition.SetY(Renderer::GetCursorPosition().GetY());
		Renderer::SetCursorPosition(cursorPosition);
		if(Renderer::DrawButton(Utility::Text::Apply, SmallButtonSize) && 
		   UserFunctionName[0] != '\0')
		{
			userFunction.Name = UserFunctionName;

			userFunction.BackSlots.erase(userFunction.BackSlots.begin()+1, userFunction.BackSlots.end());
			for( auto& userFunctionInput : UserFunctionInputs )
			{
				userFunction.BackSlots.emplace_back(userFunctionInput);
			}

			userFunction.FrontSlots.erase(userFunction.FrontSlots.begin()+1, userFunction.FrontSlots.end());
			for( auto& userFunctionOutput : UserFunctionOutputs )
			{
				userFunction.FrontSlots.emplace_back(userFunctionOutput);
			}

			Utility::Core::FunctionManager::Edit(m_CurrentUserFunction, userFunction);

			// Send edit message to update spawned nodes on code board
			Service::EditUserFunctionData data = {m_CurrentUserFunction};
			SendMessage(Service::MessageType::EditUserFunction, &data);
		}
		Renderer::Spacing();
		cursorPosition.SetY(Renderer::GetCursorPosition().GetY());
		Renderer::SetCursorPosition(cursorPosition);
		if(Renderer::DrawButton(Utility::Text::Remove, SmallButtonSize))
		{
			Service::RemoveUserFunctionData data = {m_CurrentUserFunction};
			SendMessage(Service::MessageType::RemoveUserFunction, &data);

			m_CurrentUserFunction = -1;
		}
	}
}
void Details::OnMessage(Service::MessageType type, void* data)
{
	switch(type)
	{
		case Service::MessageType::NewProject:
		{
			m_CurrentUserFunction = -1;
			break;
		}
		case Service::MessageType::EditUserFunction:
		{
			if(m_CurrentUserFunction != -1)
			{
				auto userFunction = Utility::Core::FunctionManager::Get(m_CurrentUserFunction);
				if(userFunction.Name.size() == 0 || 
				   userFunction.Name[0] == '\0')
				{
					Service::RemoveUserFunctionData data = {m_CurrentUserFunction};
					SendMessage(Service::MessageType::RemoveUserFunction, &data);
				}
			}

			auto userFunctionData = static_cast<Service::EditUserFunctionData*>(data);
			
			m_CurrentUserFunction = userFunctionData->FunctionID;
			UserFunctionName[0] = '\0';
			UserFunctionInputs.erase(UserFunctionInputs.begin(), UserFunctionInputs.end());
			UserFunctionOutputs.erase(UserFunctionOutputs.begin(), UserFunctionOutputs.end());

			auto userFunction = Utility::Core::FunctionManager::Get(m_CurrentUserFunction);
			if(userFunction.Name.size() > 0)
			{
				UserFunctionName = userFunction.Name;
			}
			for( auto& it=userFunction.BackSlots.begin()+1 ; it!=userFunction.BackSlots.end() ; ++it )
			{
				UserFunctionInputs.emplace_back(*it);
			}
			for( auto& it=userFunction.FrontSlots.begin()+1 ; it!=userFunction.FrontSlots.end() ; ++it )
			{
				UserFunctionOutputs.emplace_back(*it);
			}

			break;
		}
	}
}

} // namespace Koala::Editor
