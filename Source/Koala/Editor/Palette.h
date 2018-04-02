#ifndef KOALA__EDITOR__PALETTE
#define KOALA__EDITOR__PALETTE

#include <Koala/Editor/Gfx/Panel.h>

namespace Koala::Editor {

class Palette final : public Gfx::Panel
{
public:
	Palette(const Tool::Window& window);

private:
	virtual void OnGui() override;

};

} // namespace Koala::Editor

#endif // KOALA__EDITOR__PALETTE
