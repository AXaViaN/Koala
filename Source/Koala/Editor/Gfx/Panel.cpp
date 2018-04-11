#include <Koala/Editor/Gfx/Panel.h>
#include <imgui/imgui.h>
#include <string>

namespace Koala::Editor::Gfx {

Panel::Panel(const Tool::Window& window, Vector2 startPoint, Vector2 endPoint) : 
	m_Window(window),
	m_StartPoint(startPoint),
	m_EndPoint(endPoint)
{ }

void Panel::Update()
{
	char title[128];
	std::sprintf(title, "Panel - %p", this);

	ImGui::SetNextWindowPos(ImVec2(m_StartPoint.GetX() * m_Window.GetWidth(), 
								   m_StartPoint.GetY() * m_Window.GetHeight()));
	ImGui::SetNextWindowSize(ImVec2((m_EndPoint-m_StartPoint).GetX() * m_Window.GetWidth(), 
									(m_EndPoint-m_StartPoint).GetY() * m_Window.GetHeight()));
	ImGui::Begin(title, nullptr, 
				 ImGuiWindowFlags_NoCollapse | 
				 ImGuiWindowFlags_NoMove | 
				 ImGuiWindowFlags_NoResize | 
				 ImGuiWindowFlags_NoTitleBar | 
				 m_AdditionalFlags);
	{
		ImGui::SetCursorPos({0, 0});
		OnGui();
	}
	ImGui::End();
}

void Panel::DisableScrollBar()
{
	m_AdditionalFlags |= ImGuiWindowFlags_NoScrollbar;
}

} // namespace Koala::Editor::Gfx
