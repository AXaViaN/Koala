#ifndef KOALA__EDITOR__CONTROL_PANEL
#define KOALA__EDITOR__CONTROL_PANEL

#include <Koala/Editor/Gfx/Panel.h>

namespace Koala::Editor {

class ControlPanel final : public Gfx::Panel
{
public:
	ControlPanel(const Tool::Window& window);

private:
	virtual void OnGui() override;

};

} // namespace Koala::Editor

#endif // KOALA__EDITOR__CONTROL_PANEL
