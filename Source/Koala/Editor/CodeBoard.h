#ifndef KOALA__EDITOR__CODE_BOARD
#define KOALA__EDITOR__CODE_BOARD

#include <Koala/Editor/Gfx/Panel.h>
#include <Koala/Utility/Core/Node.h>
#include <vector>
#include <string>

namespace Koala::Editor {

class CodeBoard final : public Gfx::Panel
{
public:
	CodeBoard(const Tool::Window& window);

private:
	struct SceneNode;

private:
	virtual void OnGui() override;

private:
	std::vector<std::pair<std::string, std::vector<SceneNode>>> m_FunctionList;
	size_t m_SelectedFunction;

};

struct CodeBoard::SceneNode
{
	Utility::Core::Node Node = Utility::Core::Node(0);
	Gfx::Vector2 Position;
};

} // namespace Koala::Editor

#endif // KOALA__EDITOR__CODE_BOARD
