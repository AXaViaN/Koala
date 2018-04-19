#ifndef KOALA__EDITOR__CODE_BOARD
#define KOALA__EDITOR__CODE_BOARD

#include <Koala/Editor/Gfx/Panel.h>
#include <vector>
#include <string>

namespace Koala::Editor {

class CodeBoard final : public Gfx::Panel
{
public:
	CodeBoard(const Tool::Window& window);

private:
	virtual void OnGui() override;

private:
	std::vector<std::string> m_FunctionList;
	size_t m_SelectedFunction;

};

} // namespace Koala::Editor

#endif // KOALA__EDITOR__CODE_BOARD
