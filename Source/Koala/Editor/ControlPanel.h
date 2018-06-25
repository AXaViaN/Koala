#ifndef KOALA__EDITOR__CONTROL_PANEL
#define KOALA__EDITOR__CONTROL_PANEL

#include <Koala/Editor/Gfx/Panel.h>
#include <Koala/Utility/Core/FunctionManager.h>

namespace Koala::Editor {

class ControlPanel final : public Gfx::Panel
{
public:
	ControlPanel(const Tool::Window& window);

private:
	virtual void OnGui() override;
	virtual void OnMessage(Service::MessageType type, void* data) override;

private:
	std::vector<Utility::Core::FunctionID> m_UserFunctionIDs;

};

} // namespace Koala::Editor

#endif // KOALA__EDITOR__CONTROL_PANEL
