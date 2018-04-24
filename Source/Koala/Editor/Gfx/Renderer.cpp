#include <Koala/Editor/Gfx/Renderer.h>
#include <GL/glew.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include <algorithm>

namespace Koala::Editor::Gfx {

static const Color ErrorColor = Color(1.0f, 0.0f, 1.0f);
static ImVec2 GetImVec2(const Vector2& vec)
{
	return {vec.GetX(), vec.GetY()};
}
static ImVec4 GetImVec4(const Color& color)
{
	return {color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha()};
}
static ImU32 GetImU32(const Color& color)
{
	return IM_COL32(color.GetRed()*255, color.GetGreen()*255, color.GetBlue()*255, color.GetAlpha()*255);
}
static ImVec2 GetRealImVec(const Vector2& vec)
{
	return {ImGui::GetWindowSize().x * vec.GetX(), 
			ImGui::GetWindowSize().y * vec.GetY()};
}
static ImVec2 operator+(const ImVec2& first, const ImVec2& second)
{
	return {first.x + second.x, 
			first.y + second.y};
}
static ImVec2& operator+=(ImVec2& first, const ImVec2& second)
{
	first = first + second;
	return first;
}
static ImVec2 operator-(const ImVec2& first, const ImVec2& second)
{
	return {first.x - second.x, 
			first.y - second.y};
}
static ImVec2& operator-=(ImVec2& first, const ImVec2& second)
{
	first = first - second;
	return first;
}

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
	glClearColor(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
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
void Renderer::SetCursorPosition(const Vector2& position)
{
	ImGui::SetCursorPos(GetRealImVec(position));
}
Vector2 Renderer::GetCursorPosition()
{
	Vector2 pos;
	pos.SetX(ImGui::GetCursorPos().x / ImGui::GetWindowSize().x);
	pos.SetY(ImGui::GetCursorPos().y / ImGui::GetWindowSize().y);

	return pos;
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
bool Renderer::DrawIconButton(Utility::Icon icon, float height, const Color& bgColor)
{
	float width = (height * ImGui::GetWindowSize().y) / ImGui::GetWindowSize().x;
	return DrawIconButton(icon, Vector2(width, height), bgColor);
}
bool Renderer::DrawIconButton(Utility::Icon icon, const Vector2& size, const Color& bgColor)
{
	ImVec2 buttonSize;
	buttonSize.x = ImGui::GetWindowSize().x * size.GetX();
	buttonSize.y = ImGui::GetWindowSize().y * size.GetY();

	ImVec4 backgroundColor = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
	if(bgColor.GetAlpha() > 0.0f)
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

void Renderer::DrawNode(const Utility::Core::Node& node, const Vector2& position)
{
	static const Vector2 NodeBaseSize = Vector2(0.25f, 0.075f);
	static const float NodeItemHeight = 0.05f;

	const float SlotHeight = ImGui::GetWindowSize().y * NodeItemHeight;

	ImVec2 nodeSize = GetRealImVec(NodeBaseSize);
	nodeSize.y += std::max(node.GetBackSlots().size(), node.GetFrontSlots().size()) * SlotHeight;
	
	ImGui::PushID(&node);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0.5));
	SetCursorPosition(position);
	ImGui::BeginChild(node.GetName().c_str(), nodeSize, true, 
					  ImGuiWindowFlags_AlwaysAutoResize | 
					  ImGuiWindowFlags_NoCollapse | 
					  ImGuiWindowFlags_NoTitleBar | 
					  ImGuiWindowFlags_NoScrollbar);
	{
		ImGui::Text(node.GetName().c_str());

		Spacing();
		DrawSeperator();
		Spacing(4);

		const Vector2 SlotStartPosition = GetCursorPosition();

		enum class SlotSide
		{
			Front,
			Back
		};
		auto drawSlots = [&SlotStartPosition, &SlotHeight](const std::vector<Utility::Core::Slot> slots, SlotSide slotSide)
		{
			for( size_t i=0 ; i<slots.size() ; ++i )
			{
				auto& slot = slots[i];

				ImGui::PushID(&slot);

				bool isFlow = false;
				ImU32 slotColor;
				ImVec2 slotPosition = ImGui::GetWindowPos();
				if(slotSide == SlotSide::Back)
				{
					slotPosition += GetRealImVec(SlotStartPosition);
				}
				else
				{
					auto startPositionX = 1.0f - SlotStartPosition.GetX();
					slotPosition += GetRealImVec({startPositionX, SlotStartPosition.GetY()});
					slotPosition.x -= SlotHeight / 2.0f;
				}
				slotPosition.y += i * SlotHeight;

				// Select color based on the variable type
				auto& slotVariableType = slot.GetVariable().GetVariableType();
				if(slotVariableType == Utility::Core::VariableType::None)
				{
					// It is a flow
					slotColor = GetImU32(Color(1.0f, 1.0f, 1.0f));
					isFlow = true;
				}
				else if(slotVariableType == Utility::Core::VariableType::Float64)
				{
					slotColor = GetImU32(Color(0.5f, 0.7f, 1.0f));
				}
				else if(slotVariableType == Utility::Core::VariableType::String)
				{
					slotColor = GetImU32(Color(0.5f, 1.0f, 0.7f));
				}
				else
				{
					slotColor = GetImU32(ErrorColor);
				}
				
				// Draw slot port
				if(isFlow)
				{
					auto position0 = slotPosition;
					auto position1 = slotPosition + ImVec2(SlotHeight/2.0f, SlotHeight/2.0f);
					if(slot.IsConnected())
					{
						ImGui::GetWindowDrawList()->AddRectFilled(position0, position1, slotColor);
					}
					else
					{
						ImGui::GetWindowDrawList()->AddRect(position0, position1, slotColor);
					}
				}
				else
				{
					auto radius = SlotHeight / 4.0f;
					auto position = slotPosition + ImVec2(radius, radius);
					if(slot.IsConnected())
					{
						ImGui::GetWindowDrawList()->AddCircleFilled(position, radius, slotColor);
					}
					else
					{
						ImGui::GetWindowDrawList()->AddCircle(position, radius, slotColor);
					}
				}
				
				ImGui::PopID();
			}
		};

		drawSlots(node.GetFrontSlots(), SlotSide::Front);
		drawSlots(node.GetBackSlots(), SlotSide::Back);
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopID();
}

} // namespace Koala::Editor::Gfx
