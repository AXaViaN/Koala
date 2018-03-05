#include <Koala/Editor/Gfx/Renderer.h>
#include <GL/glew.h>

namespace Koala::Editor::Gfx {

void Renderer::ClearViewport(const Color& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

} // namespace Koala::Editor::Gfx
