#ifndef KOALA__EDITOR__PALETTE
#define KOALA__EDITOR__PALETTE

#include <Koala/Editor/Gfx/Panel.h>
#include <Koala/Utility/Resource.h>
#include <vector>
#include <map>

namespace Koala::Editor {

class Palette final : public Gfx::Panel
{
public:
	Palette(const Tool::Window& window);

private:
	virtual void OnGui() override;

private:
	std::map<Utility::Text, std::vector<Utility::Text>> m_FunctionList;

};

} // namespace Koala::Editor

#endif // KOALA__EDITOR__PALETTE
