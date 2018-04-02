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

};

} // namespace Koala::Editor

#endif // KOALA__EDITOR__DETAILS
