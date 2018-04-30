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
	struct Function;

private:
	virtual void OnGui() override;
	virtual void OnInput(Service::InputMessageType type, const Service::InputMessageData& data) override;

private:
	const Tool::Window& m_Window;

	std::vector<Function> m_FunctionList;
	size_t m_SelectedFunction;

};

struct CodeBoard::SceneNode
{
	Utility::Core::Node Node = Utility::Core::Node(0);
	Gfx::Vector2 Position;
};
struct CodeBoard::Function
{
	std::string Name;
	std::vector<SceneNode> SceneNodes;

	Gfx::Vector2 DragOffset;
};

} // namespace Koala::Editor

#endif // KOALA__EDITOR__CODE_BOARD
