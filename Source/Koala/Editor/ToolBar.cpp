#include <Koala/Editor/ToolBar.h>
#include <Koala/Editor/Gfx/Renderer.h>

namespace Koala::Editor {

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

	const float ButtonHeight = 0.75f;
	const Color ButtonColor = Color(0.75f, 0.75f, 0.75f);

	Renderer::SetCursorPosition({0.005f, 0.07f});
	if(Renderer::DrawIconButton(Utility::Icon::New, ButtonHeight, ButtonColor))
	{
		auto data = GenerateLogMessageData();
		data.Message = "New button is pressed!";

		SendMessage(Service::MessageType::LogInfo, &data);
	}
	Renderer::DrawSameLine();
	if(Renderer::DrawIconButton(Utility::Icon::Open, ButtonHeight, ButtonColor))
	{
		auto data = GenerateLogMessageData();
		data.Message = "Open button is pressed!";

		SendMessage(Service::MessageType::LogInfo, &data);
	}
	Renderer::DrawSameLine();
	if(Renderer::DrawIconButton(Utility::Icon::Save, ButtonHeight, ButtonColor))
	{
		auto data = GenerateLogMessageData();
		data.Message = "Save button is pressed!";

		SendMessage(Service::MessageType::LogInfo, &data);
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
		auto data = GenerateLogMessageData();
		data.Message = "Run button is pressed!";

		SendMessage(Service::MessageType::LogInfo, &data);
	}
	{
		Renderer::DrawSameLine();
		const float NextPositionX = 2*ScreenMiddle.GetX() - Renderer::GetCursorPosition().GetX();
		Renderer::SetCursorPosition({NextPositionX, ScreenMiddle.GetY()});
	}
	if(Renderer::DrawIconButton(Utility::Icon::Compile, ButtonHeight, ButtonColor))
	{
		auto data = GenerateLogMessageData();
		data.Message = "Compile button is pressed!";

		SendMessage(Service::MessageType::LogInfo, &data);
	}
}

} // namespace Koala::Editor
