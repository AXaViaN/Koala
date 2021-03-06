#ifndef KOALA__EDITOR__GFX__RENDERER
#define KOALA__EDITOR__GFX__RENDERER

#include <Koala/Editor/Gfx/Color.h>
#include <Koala/Editor/Gfx/Vector2.h>
#include <Koala/Utility/Resource.h>
#include <Koala/Utility/Core/Node.h>
#include <string>

namespace Koala::Editor::Gfx {

class Renderer
{
public:
	static void Initialize();
	static void Terminate();

	static void ClearViewport(const Color& color);
	static void Update();

	static void DrawGuiDemo();

	static void DrawSameLine(float spacing=0.0025f);
	static void Spacing(size_t count=1);
	static void DrawSeperator();
	static void SetCursorPosition(const Vector2& position);
	static Vector2 GetCursorPosition();

	static void OpenPopup(const std::string& title);
	static void ClosePopup();
	static bool BeginPopup(const std::string& title);
	static void EndPopup();

	static bool DrawText(const std::string& str);
	static bool DrawText(Utility::Text text);
	static bool DrawButton(const std::string& str, float height, bool highlight=false);
	static bool DrawButton(Utility::Text text, float height, bool highlight=false);
	static bool DrawButton(const std::string& str, const Vector2& size, bool highlight=false);
	static bool DrawButton(Utility::Text text, const Vector2& size, bool highlight=false);
	static bool DrawIconButton(Utility::Icon icon, float height, const Color& bgColor=Color(0,0,0,0));
	static bool DrawIconButton(Utility::Icon icon, const Vector2& size, const Color& bgColor=Color(0,0,0,0));
	
	static void PushItemID(const void* id);
	static void PushItemID(int id);
	static void PopItemID();

	static void InputText(std::string* str, Utility::Text defaultText=Utility::Text::Empty);
	static void InputInt(int* value);
	static void ComboBox(int* selectedIdx, const std::vector<std::string>& elements);

	static bool DrawTree(Utility::Text text);
	static void EndTree();

	static void DrawNode(Utility::Core::Node& node, const Vector2& position);
	static void DrawFrontConnections(const Utility::Core::Node& node, const Vector2& position);
	static void DrawConnectionToMouse(const Utility::Core::Node& node, const Vector2& position, Utility::Core::SlotSide slotSide, size_t slotIndex);

};

} // namespace Koala::Editor::Gfx

#endif // KOALA__EDITOR__GFX__RENDERER
