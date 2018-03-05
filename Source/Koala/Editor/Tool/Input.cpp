#include <Koala/Editor/Tool/Input.h>
#include <GLFW/glfw3.h>

namespace Koala::Editor::Tool {

void Input::Update()
{
	glfwPollEvents();
}

} // namespace Koala::Editor::Tool
