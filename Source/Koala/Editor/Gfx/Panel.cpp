#include <Koala/Editor/Gfx/Panel.h>
#include <Koala/Editor/Gfx/Renderer.h>
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
void Panel::DisablePanelInput()
{
	m_AdditionalFlags |= ImGuiWindowFlags_NoInputs;
}

void Panel::BeginSubPanel(bool enableInputs)
{
	BeginSubPanel(Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f), enableInputs);
}
void Panel::BeginSubPanel(Vector2 startPosition, Vector2 endPosition, bool enableInputs)
{
	Vector2 subPanelSize = endPosition - startPosition;
	ImVec2 subPanelRealSize = ImVec2(ImGui::GetWindowSize().x * subPanelSize.GetX(), 
									 ImGui::GetWindowSize().y * subPanelSize.GetY());

	unsigned int flags = ImGuiWindowFlags_AlwaysAutoResize | 
						 ImGuiWindowFlags_NoCollapse | 
						 ImGuiWindowFlags_NoTitleBar | 
						 ImGuiWindowFlags_NoScrollbar;
	if(enableInputs == false)
	{
		flags |= ImGuiWindowFlags_NoInputs;
	}

	ImGui::PushID(this);
	Renderer::SetCursorPosition(startPosition);
	ImGui::BeginChild("", subPanelRealSize, false, flags);
}
void Panel::EndSubPanel()
{
	ImGui::EndChild();
	ImGui::PopID();
}

} // namespace Koala::Editor::Gfx
