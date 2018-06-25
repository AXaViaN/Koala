#ifndef KOALA__EDITOR__DETAILS
#define KOALA__EDITOR__DETAILS

#include <Koala/Editor/Gfx/Panel.h>

namespace Koala::Editor {

class Details final : public Gfx::Panel
{
public:
	Details(const Tool::Window& window);

private:
	virtual void OnGui() override;
	virtual void OnMessage(Service::MessageType type, void* data) override;

private:
	Utility::Core::FunctionID m_CurrentUserFunction = -1;

};

} // namespace Koala::Editor

#endif // KOALA__EDITOR__DETAILS
