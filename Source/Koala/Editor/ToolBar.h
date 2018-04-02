#ifndef KOALA__EDITOR__TOOL_BAR
#define KOALA__EDITOR__TOOL_BAR

#include <Koala/Editor/Gfx/Panel.h>

namespace Koala::Editor {

class ToolBar final : public Gfx::Panel
{
public:
	ToolBar(const Tool::Window& window);

private:
	virtual void OnGui() override;

};

} // namespace Koala::Editor

#endif // KOALA__EDITOR__TOOL_BAR
