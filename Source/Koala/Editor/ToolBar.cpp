#include <Koala/Editor/ToolBar.h>
#include <Koala/Editor/CodeBoard.h>
#include <Koala/Editor/Gfx/Renderer.h>
#include <Koala/Utility/Platform.h>
#include <Koala/Utility/Serialization.h>
#include <Koala/Utility/Extra/Util.h>
#include <algorithm>

namespace Koala::Editor {

static Service::SaveProjectData ConvertSerializationData(const Utility::Serialization::Data& data);
static Utility::Serialization::Data ConvertSerializationData(const Service::SaveProjectData& data);

ToolBar::ToolBar(const Tool::Window& window) :
	Gfx::Panel(window, 
			   Gfx::Vector2(0.0f, 0.0f), 
			   Gfx::Vector2(1.0f, CodeBoardStartPosition().GetY()))
{
	DisableScrollBar();
}

void ToolBar::OnGui()
{
	using namespace Gfx;

	constexpr float ButtonHeight = 0.75f;
	const Color ButtonColor = Color(0.75f, 0.75f, 0.75f);

	static std::string currentSavePath;
	static bool openSaveLoadPopup = false;
	static bool compileProject = false;
	static bool runProject = false;
	const Vector2 PopupButtonSize = Vector2(0.47f, 0.25f);

	// Icons
	Renderer::SetCursorPosition({0.005f, 0.07f});
	if(Renderer::DrawIconButton(Utility::Icon::New, ButtonHeight, ButtonColor))
	{
		Utility::Core::FunctionManager::RemoveAllUserFunctions();
		SendMessage(Service::MessageType::NewProject, nullptr);
	}
	Renderer::DrawSameLine();
	if(Renderer::DrawIconButton(Utility::Icon::Open, ButtonHeight, ButtonColor))
	{
		Renderer::OpenPopup(Utility::Resource::GetText(Utility::Text::Open));
		openSaveLoadPopup = true;
	}
	Renderer::DrawSameLine();
	if(Renderer::DrawIconButton(Utility::Icon::Save, ButtonHeight, ButtonColor))
	{
		Renderer::OpenPopup(Utility::Resource::GetText(Utility::Text::Save));
		openSaveLoadPopup = true;
	}
	Renderer::DrawSameLine();
	if(Renderer::DrawIconButton(Utility::Icon::Settings, ButtonHeight, ButtonColor))
	{
		auto data = GenerateLogMessageData();
		data.Message = "Settings button is pressed!";

		SendMessage(Service::MessageType::LogInfo, &data);
	}

	const Vector2 ScreenMiddle = {0.5f, 0.07f};
	Renderer::SetCursorPosition(ScreenMiddle);
	if(Renderer::DrawIconButton(Utility::Icon::Run, ButtonHeight, ButtonColor))
	{
		Renderer::OpenPopup(Utility::Resource::GetText(Utility::Text::Save));
		openSaveLoadPopup = true;
		runProject = true;
	}
	{
		Renderer::DrawSameLine();
		const float NextPositionX = 2*ScreenMiddle.GetX() - Renderer::GetCursorPosition().GetX();
		Renderer::SetCursorPosition({NextPositionX, ScreenMiddle.GetY()});
	}
	if(Renderer::DrawIconButton(Utility::Icon::Compile, ButtonHeight, ButtonColor))
	{
		Renderer::OpenPopup(Utility::Resource::GetText(Utility::Text::Save));
		openSaveLoadPopup = true;
		compileProject = true;
	}

	// Pop-up
	if(openSaveLoadPopup)
	{
		// Both pop-ups are the same, except for the Save/Open button
		// Get button text
		Utility::Text buttonText;
		if(Renderer::BeginPopup(Utility::Resource::GetText(Utility::Text::Save)))
		{
			buttonText = Utility::Text::Save;
		}
		else
		{
			Renderer::BeginPopup(Utility::Resource::GetText(Utility::Text::Open));
			buttonText = Utility::Text::Open;
		}

		// Save path input
		Renderer::DrawText(Utility::Text::FileName);
		Renderer::InputText(&currentSavePath);
		Renderer::Spacing(4);

		// Convert path to desired format
		std::string projectPath = std::string(currentSavePath.c_str());
		std::replace(projectPath.begin(), projectPath.end(), '\\', '/');
		projectPath.erase(0, projectPath.find_first_not_of('/'));

		// Draw Save/Open button
		if(Renderer::DrawButton(buttonText, PopupButtonSize) && 
		   projectPath.size() != 0 && 
		   projectPath.back() != '/')
		{
			projectPath += ".kproj";

			if(buttonText == Utility::Text::Save)
			{
				// Save project
				std::string projectName;
				{
					size_t nameStartIdx = projectPath.find_last_of('/')+1;
					if(nameStartIdx != projectPath.npos)
					{
						projectName = projectPath.substr(nameStartIdx);
					}
					else
					{
						projectName = projectPath;
					}
				}
				projectName.erase(projectName.find_last_of('.'));

				Service::SaveProjectData data;
				data.ProjectName = projectName;
				SendMessage(Service::MessageType::SaveProject, &data);

				Utility::Serialization::SaveProject(projectPath, ConvertSerializationData(data));

				if(compileProject)
				{
					compileProject = false;

					Utility::Extra::Util::RunExternalProgram("KoalaCompiler", projectPath);
				}
				if(runProject)
				{
					runProject = false;
					
					// Compile before running
					Utility::Extra::Util::RunExternalProgram("KoalaCompiler", projectPath, false);

					std::string koaPath = projectPath;
					koaPath.erase(koaPath.find_last_of('.'));
					koaPath += ".koa";
					Utility::Extra::Util::RunExternalProgram("KoalaVM", koaPath);
				}
			}
			else
			{
				// Load project
				auto serializationData = Utility::Serialization::LoadProject(projectPath);
				if(serializationData.IsDataValid)
				{
					auto& data = ConvertSerializationData(serializationData);
					SendMessage(Service::MessageType::LoadProject, &data);
				}
			}

			Renderer::ClosePopup();
			openSaveLoadPopup = false;
		}
		Renderer::DrawSameLine();
		if(Renderer::DrawButton(Utility::Text::Cancel, PopupButtonSize))
		{
			Renderer::ClosePopup();
			openSaveLoadPopup = false;
			compileProject = false;
			runProject = false;
		}

		Renderer::EndPopup();
	}
}

static Service::SaveProjectData ConvertSerializationData(const Utility::Serialization::Data& data)
{
	Service::SaveProjectData otherData;

	otherData.ProjectName = data.ProjectName;
	otherData.SelectedFunction = data.SelectedFunction;
	for( auto& function : data.Functions )
	{
		auto& otherFunction = otherData.Functions.emplace_back();

		otherFunction.CoreNodeCount = function.CoreNodeCount;
		otherFunction.DragOffset = Gfx::Vector2(function.DragOffset.X, function.DragOffset.Y);
		for( auto& sceneNode : function.SceneNodes )
		{
			auto& otherSceneNode = otherFunction.SceneNodes.emplace_back();
			
			otherSceneNode.Node = sceneNode.Node;
			otherSceneNode.Position = Gfx::Vector2(sceneNode.Position.X, sceneNode.Position.Y);
		}
	}

	return otherData;
}
static Utility::Serialization::Data ConvertSerializationData(const Service::SaveProjectData& data)
{
	Utility::Serialization::Data otherData;

	otherData.ProjectName = data.ProjectName;
	otherData.SelectedFunction = data.SelectedFunction;
	for( auto& function : data.Functions )
	{
		auto& otherFunction = otherData.Functions.emplace_back();

		otherFunction.CoreNodeCount = function.CoreNodeCount;
		otherFunction.DragOffset.X = function.DragOffset.GetX();
		otherFunction.DragOffset.Y = function.DragOffset.GetY();
		for( auto& sceneNode : function.SceneNodes )
		{
			auto& otherSceneNode = otherFunction.SceneNodes.emplace_back();

			otherSceneNode.Node = sceneNode.Node;
			otherSceneNode.Position.X = sceneNode.Position.GetX();
			otherSceneNode.Position.Y = sceneNode.Position.GetY();
		}
	}

	return otherData;
}

} // namespace Koala::Editor
