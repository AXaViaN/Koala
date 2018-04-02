#include <Koala/Editor/Gfx/Renderer.h>
#include <GL/glew.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

namespace Koala::Editor::Gfx {

void Renderer::Initialize()
{
	ImGui::CreateContext();
	ImGui::GetIO().IniFilename = nullptr;

	ImGui::StyleColorsDark();
	ImGui::GetIO().FontGlobalScale = 1.4f;
	ImGui::GetStyle().WindowRounding = 0.0f;
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
void Renderer::DrawText(const std::string& text)
{
	ImGui::Text(text.c_str());
}

} // namespace Koala::Editor::Gfx
