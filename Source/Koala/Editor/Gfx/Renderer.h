#ifndef KOALA__EDITOR__GFX__RENDERER
#define KOALA__EDITOR__GFX__RENDERER

#include <Koala/Editor/Gfx/Color.h>
#include <Koala/Editor/Gfx/Vector2.h>
#include <Koala/Utility/Resource.h>
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

	static void SetCursorPosition(const Vector2& position);
	static void DrawSameLine(float spacing=0.0025f);
	static void Spacing(size_t count=1);
	static void DrawSeperator();

	static void DrawText(const std::string& str);
	static void DrawText(Utility::Text text);
	static bool DrawButton(const std::string& str, const Vector2& size, bool highlight=false);
	static bool DrawButton(Utility::Text text, const Vector2& size, bool highlight=false);
	static bool DrawIconButton(Utility::Icon icon, const Vector2& size, const Color& bgColor=Color(0,0,0,0));

	static void InputText(std::string* str, Utility::Text defaultText=Utility::Text::Empty);

	static bool DrawTree(Utility::Text text);
	static void EndTree();

};

} // namespace Koala::Editor::Gfx

#endif // KOALA__EDITOR__GFX__RENDERER
