#include <Koala/Editor/Gfx/Renderer.h>
#include <Koala/Editor/Service/MessageSender.h>
#include <GL/glew.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include <algorithm>

namespace Koala::Editor::Gfx {

static const Color ErrorColor = Color(1.0f, 0.0f, 1.0f);
static const Vector2 NodeBaseSize = Vector2(0.3f, 0.075f);
static constexpr float NodeItemHeight = 0.04f;
static constexpr float LineThickness = 2.0f;
static constexpr int ImGuiLeftMouse = 0;
static constexpr int ImGuiRightMouse = 1;
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
static Vector2 GetVector2(const ImVec2& vec)
{
	return {vec.x, vec.y};
}
static Vector2 GetLogicalVector2(const ImVec2& vec)
{
	return {vec.x / ImGui::GetWindowSize().x, 
			vec.y / ImGui::GetWindowSize().y};
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

// Node rendering helpers
static Vector2 g_LastSlotStartPosition;
static ImVec2 g_LastSlotWindowSize;
static ImVec2 GetSlotPosition(const ImVec2& windowPosition, float slotHeight, Utility::Core::SlotSide slotSide, size_t index)
{
	ImVec2 slotPosition = windowPosition;
	if(slotSide == Utility::Core::SlotSide::Back)
	{
		slotPosition += {g_LastSlotWindowSize.x * g_LastSlotStartPosition.GetX(), 
						 g_LastSlotWindowSize.y * g_LastSlotStartPosition.GetY()};
	}
	else
	{
		Vector2 slotStartPosition = {1.0f - g_LastSlotStartPosition.GetX(), g_LastSlotStartPosition.GetY()};
		slotPosition += {g_LastSlotWindowSize.x * slotStartPosition.GetX(), 
						 g_LastSlotWindowSize.y * slotStartPosition.GetY()};
		slotPosition.x -= slotHeight / 2.0f;
	}
	slotPosition.y += index * slotHeight;

	return slotPosition;
};
static Color GetSlotColor(Utility::Core::VariableType slotVariableType, bool* isFlow=nullptr)
{
	if(slotVariableType == Utility::Core::VariableType::None)
	{
		if(isFlow != nullptr)
		{
			*isFlow = true;
		}

		return Color(1.0f, 1.0f, 1.0f);
	}
	else if(slotVariableType == Utility::Core::VariableType::Float64)
	{
		return Color(0.5f, 0.7f, 1.0f);
	}
	else if(slotVariableType == Utility::Core::VariableType::String)
	{
		return Color(0.5f, 1.0f, 0.7f);
	}
	else
	{
		return ErrorColor;
	}
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

void Renderer::DrawNode(Utility::Core::Node& node, const Vector2& position)
{
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

		g_LastSlotStartPosition = GetCursorPosition();
		g_LastSlotWindowSize = ImGui::GetWindowSize();

		bool isRightClickHandled = false;

		auto drawSlots = [&node, &SlotHeight, &isRightClickHandled](std::vector<Utility::Core::Slot>& slots, Utility::Core::SlotSide slotSide)
		{
			for( size_t i=0 ; i<slots.size() ; ++i )
			{
				auto& slot = slots[i];

				ImGui::PushID(&slot);

				bool isFlow = false;
				auto& slotVariableType = slot.GetVariable().GetVariableType();
				Color slotColor = GetSlotColor(slotVariableType, &isFlow);
				ImVec2 slotPosition = GetSlotPosition(ImGui::GetWindowPos(), SlotHeight, slotSide, i);

				// Draw slot port
				auto radius = SlotHeight / 4.0f;
				auto position0 = slotPosition;
				auto position1 = slotPosition + ImVec2(SlotHeight/2.0f, SlotHeight/2.0f);
				auto positionCenter = slotPosition + ImVec2(radius, radius);
				if(isFlow)
				{
					auto color = GetImU32(slotColor);
					if(slot.IsConnected())
					{
						ImGui::GetWindowDrawList()->AddRectFilled(position0, position1, color);
					}
					else
					{
						ImGui::GetWindowDrawList()->AddRect(position0, position1, color);
					}
				}
				else
				{
					auto color = GetImU32(slotColor);
					if(slot.IsConnected())
					{
						ImGui::GetWindowDrawList()->AddCircleFilled(positionCenter, radius, color);
					}
					else
					{
						ImGui::GetWindowDrawList()->AddCircle(positionCenter, radius, color);
					}
				}

				// Slot input
				static bool isConnecting = false;
				if(ImGui::IsMouseHoveringRect(position0, position1))
				{
					if(ImGui::IsMouseDown(ImGuiLeftMouse) &&
					   isConnecting == false)
					{
						Service::ConnectionBeginData data = {node, slotSide, i};
						Service::MessageSender::Send(Service::MessageType::ConnectionBegin, &data);
						isConnecting = true;
					}
					else if(ImGui::IsMouseReleased(ImGuiLeftMouse))
					{
						Service::ConnectionEndData data = {node, slotSide, i};
						Service::MessageSender::Send(Service::MessageType::ConnectionEnd, &data);
						isConnecting = false;
					}
					else if(ImGui::IsMouseClicked(ImGuiRightMouse) &&
							isConnecting == false)
					{
						Service::ConnectionRemoveData data = {node, slotSide, i};
						Service::MessageSender::Send(Service::MessageType::ConnectionRemove, &data);
						isRightClickHandled = true;
					}
				}
				if(ImGui::IsMouseReleased(ImGuiLeftMouse) &&
				   isConnecting)
				{
					Service::MessageSender::Send(Service::MessageType::ConnectionCancel, nullptr);
					isConnecting = false;
				}
				
				// Draw variable name
				Vector2 slotNamePosition;
				{
					ImVec2 cursorPosition = slotPosition - ImGui::GetWindowPos();
					ImVec2 cursorOffset = ImVec2(SlotHeight*0.75f, -SlotHeight*0.05f);
					if(slotSide == Utility::Core::SlotSide::Front)
					{
						cursorOffset.x *= -0.25f;
					}
					slotNamePosition = GetLogicalVector2(cursorPosition + cursorOffset);
				}

				SetCursorPosition(slotNamePosition);
				{
					auto& variableName = slot.GetVariable().GetName();
					size_t letterCount = variableName.size();
					float textWidth = GetLogicalVector2(ImGui::CalcTextSize(variableName.c_str())).GetX();
					float widthDifference = 0.0f;
					if(slotSide == Utility::Core::SlotSide::Back)
					{
						widthDifference = textWidth - (0.25f - slotNamePosition.GetX());
					}
					else
					{
						widthDifference = (0.5f + textWidth) - slotNamePosition.GetX();
						if(widthDifference > 0.0f)
						{
							SetCursorPosition({0.5f, slotNamePosition.GetY()});
						}
						else
						{
							SetCursorPosition({slotNamePosition.GetX() - textWidth, slotNamePosition.GetY()});
						}
					}

					std::string format = "%.*s";
					if(widthDifference > 0.0f)
					{
						letterCount = static_cast<size_t>(letterCount * (1.0f - (widthDifference / textWidth)));

						letterCount -= 2;
						format += "..";
					}

					ImGui::TextColored(GetImVec4(slotColor), format.c_str(), letterCount, variableName.c_str());
				}

				// Draw input field
				if(slotSide == Utility::Core::SlotSide::Back && 
				   slotVariableType != Utility::Core::VariableType::None &&
				   slot.IsConnected() == false)
				{
					DrawSameLine(0.01f);

					float fieldWidth = 0.5f - GetCursorPosition().GetX();
					ImGui::PushItemWidth(GetRealImVec({fieldWidth, 0}).x);
					if(slotVariableType == Utility::Core::VariableType::String)
					{
						constexpr size_t ValueSize = 256u;
						std::string value = slot.GetVariable().GetValueString();
						value.resize(ValueSize);

						ImGui::InputText("", &value[0], ValueSize);

						slot.GetVariable().SetValueString(value);
					}
					else if(slotVariableType == Utility::Core::VariableType::Float64)
					{
						auto value = slot.GetVariable().GetValueFloat64();
						ImGui::InputDouble("", &value);
						slot.GetVariable().SetValueFloat64(value);
					}
					ImGui::PopItemWidth();
				}

				ImGui::PopID();
			}
		};

		drawSlots(node.GetFrontSlots(), Utility::Core::SlotSide::Front);
		drawSlots(node.GetBackSlots(), Utility::Core::SlotSide::Back);

		// Handle input for the node and send commands
		static Utility::Core::Node* draggingNode = nullptr;
		if(ImGui::IsWindowHovered())
		{
			if(ImGui::IsMouseDragging(ImGuiLeftMouse))
			{
				draggingNode = &node;
			}
			else if(ImGui::IsMouseClicked(ImGuiRightMouse) &&
					isRightClickHandled == false)
			{
				Service::NodeRemoveData data = {node};
				Service::MessageSender::Send(Service::MessageType::NodeRemove, &data);
			}
		}
		if(&node == draggingNode)
		{
			if(ImGui::IsMouseReleased(ImGuiLeftMouse))
			{
				draggingNode = nullptr;
			}
			else
			{
				auto mouseDrag = ImGui::GetIO().MouseDelta;

				Service::NodeMoveData data = {node, GetVector2(mouseDrag)};
				Service::MessageSender::Send(Service::MessageType::NodeMove, &data);
			}
		}

	}
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopID();
}
void Renderer::DrawFrontConnections(const Utility::Core::Node& node, const Vector2& position)
{
	const float SlotHeight = ImGui::GetWindowSize().y * NodeItemHeight;
	const ImVec2 PanelPositionRaw = ImGui::GetWindowPos();

	ImVec2 nodePosition = PanelPositionRaw + GetRealImVec(position);
	
	ImGui::PushID(&node);
	for( size_t i=0 ; i<node.GetFrontSlots().size() ; ++i )
	{
		auto& slot = node.GetFrontSlots()[i];
		ImGui::PushID(&slot);

		if(slot.IsConnected())
		{
			// Select color based on the variable type
			Color slotColor = GetSlotColor(slot.GetVariable().GetVariableType());
			
			// Get connection and other node
			auto& connections = slot.GetConnections();

			for( auto& connection : connections )
			{
				Service::RequestNodeData other = {connection.NodeID};
				Service::MessageSender::Send(Service::MessageType::RequestNode, &other);

				// Calculate the other slots position
				ImVec2 otherNodePosition = PanelPositionRaw + GetRealImVec(other.NodePosition);

				// Draw
				auto radius = SlotHeight / 4.0f;
				auto position0 = GetSlotPosition(nodePosition, SlotHeight, Utility::Core::SlotSide::Front, i) + ImVec2(radius, radius);
				auto position1 = GetSlotPosition(otherNodePosition, SlotHeight, Utility::Core::SlotSide::Back, connection.SlotIndex) + ImVec2(radius, radius);
				auto color = GetImU32(slotColor);
				ImVec2 cp0 = ImVec2(position1.x, position0.y);
				ImVec2 cp1 = ImVec2(position0.x, position1.y);
				ImGui::GetWindowDrawList()->AddBezierCurve(position0, cp0, cp1, position1, 
														   color, LineThickness);
			}
		}

		ImGui::PopID();
	}
	ImGui::PopID();
}
void Renderer::DrawConnectionToMouse(const Utility::Core::Node& node, const Vector2& position, Utility::Core::SlotSide slotSide, size_t slotIndex)
{
	const float SlotHeight = ImGui::GetWindowSize().y * NodeItemHeight;
	const ImVec2 PanelPositionRaw = ImGui::GetWindowPos();

	ImVec2 nodePosition = PanelPositionRaw + GetRealImVec(position);

	// Select color based on the variable type
	auto& slot = node.GetSlots(slotSide)[slotIndex];

	Color slotColor = GetSlotColor(slot.GetVariable().GetVariableType());

	ImVec2 mousePosition = ImGui::GetMousePos();

	// Draw
	auto radius = SlotHeight / 4.0f;
	auto position0 = GetSlotPosition(nodePosition, SlotHeight, slotSide, slotIndex) + ImVec2(radius, radius);
	auto position1 = mousePosition;
	auto color = GetImU32(slotColor);
	ImVec2 cp0 = ImVec2(position1.x, position0.y);
	ImVec2 cp1 = ImVec2(position0.x, position1.y);
	ImGui::GetWindowDrawList()->AddBezierCurve(position0, cp0, cp1, position1, 
											   color, LineThickness);
}

} // namespace Koala::Editor::Gfx
