#include <Koala/Editor/Gfx/Renderer.h>
#include <GL/glew.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

namespace Koala::Editor::Gfx {

static ImVec2 GetImVec2(const Vector2& vec);
static ImVec4 GetImVec4(const Color& color);

void Renderer::Initialize()
{
	ImGui::CreateContext();
	ImGui::GetIO().IniFilename = nullptr;

	ImGui::StyleColorsDark();
	ImGui::GetIO().FontGlobalScale = 1.4f;
	ImGui::GetStyle().WindowRounding = 0.0f;
	ImGui::PushStyleColor(ImGuiCol_Button, {1.0f, 1.0f, 1.0f, 0.5f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {1.0f, 1.0f, 1.0f, 0.7f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, {1.0f, 1.0f, 1.0f, 0.9f});
	ImGui::PushStyleColor(ImGuiCol_Header, {0.7f, 0.7f, 0.7f, 0.5f});
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, {0.7f, 0.7f, 0.7f, 0.7f});
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, {0.7f, 0.7f, 0.7f, 0.9f});
}
void Renderer::Terminate()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
}

void Renderer::ClearViewport(const Color& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplGlfwGL3_NewFrame();
}
void Renderer::Update()
{
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::DrawGuiDemo()
{
	ImGui::ShowDemoWindow();
}

void Renderer::SetCursorPosition(const Vector2& position)
{
	ImVec2 pos;
	pos.x = ImGui::GetWindowSize().x * position.GetX();
	pos.y = ImGui::GetWindowSize().y * position.GetY();

	ImGui::SetCursorPos(pos);
}
void Renderer::DrawSameLine(float spacing)
{
	ImGui::SameLine(0.0f, ImGui::GetWindowSize().x * spacing);
}
void Renderer::Spacing(size_t count)
{
	while((count--) > 0)
	{
		ImGui::Spacing();
	}
}
void Renderer::DrawSeperator()
{
	ImGui::Separator();
}

void Renderer::DrawText(const std::string& str)
{
	ImGui::Text(str.c_str());
}
void Renderer::DrawText(Utility::Text text)
{
	ImGui::Text(Utility::Resource::GetText(text).c_str());
}
bool Renderer::DrawButton(const std::string& str, const Vector2& size, bool highlight)
{
	ImVec2 buttonSize;
	buttonSize.x = ImGui::GetWindowSize().x * size.GetX();
	buttonSize.y = ImGui::GetWindowSize().y * size.GetY();

	if(highlight)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
	}
	ImGui::PushStyleColor(ImGuiCol_Text, {0.0f, 0.0f, 0.0f, 1.0f});
	bool result = ImGui::Button(str.c_str(), buttonSize);
	ImGui::PopStyleColor();
	if(highlight)
	{
		ImGui::PopStyleColor();
	}

	return result;
}
bool Renderer::DrawButton(Utility::Text text, const Vector2& size, bool highlight)
{
	return DrawButton(Utility::Resource::GetText(text), size, highlight);
}
bool Renderer::DrawIconButton(Utility::Icon icon, const Vector2& size, const Color& bgColor)
{
	ImVec2 buttonSize;
	buttonSize.x = ImGui::GetWindowSize().x * size.GetX();
	buttonSize.y = ImGui::GetWindowSize().y * size.GetY();

	ImVec4 backgroundColor = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
	if(bgColor.a > 0.0f)
	{
		backgroundColor = GetImVec4(bgColor);
	}

	return ImGui::ImageButton(Utility::Resource::GetIcon(icon), buttonSize, {0, 0}, {1, 1}, 1,
							  backgroundColor);
}

void Renderer::InputText(std::string* str, Utility::Text defaultText)
{
	if(str == nullptr)
	{
		return;
	}

	std::string& inputStr = *str;
	inputStr.resize(256u, '\0');

	ImVec2 cursorPos = ImGui::GetCursorPos();

	ImGui::PushID(&inputStr);
	ImGui::InputText("##input_text", &inputStr[0], inputStr.size());
	ImGui::PopID();

	if(inputStr[0] == '\0')
	{
		ImGui::SetCursorPos(cursorPos);
		ImGui::TextDisabled(Utility::Resource::GetText(defaultText).c_str());
	}
}

bool Renderer::DrawTree(Utility::Text text)
{
	return ImGui::TreeNode(Utility::Resource::GetText(text).c_str());
}
void Renderer::EndTree()
{
	ImGui::TreePop();
}

static ImVec2 GetImVec2(const Vector2& vec)
{
	return {vec.GetX(), vec.GetY()};
}
static ImVec4 GetImVec4(const Color& color)
{
	return {color.r, color.g, color.b, color.a};
}

} // namespace Koala::Editor::Gfx
