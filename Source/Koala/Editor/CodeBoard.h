#ifndef KOALA__EDITOR__CODE_BOARD
#define KOALA__EDITOR__CODE_BOARD

#include <Koala/Editor/Gfx/Panel.h>
#include <Koala/Utility/Core/Node.h>
#include <vector>
#include <string>

namespace Koala::Editor {

struct SceneNode;
struct Function;

class CodeBoard final : public Gfx::Panel
{
public:
	CodeBoard(const Tool::Window& window);

private:
	struct ConnectionData
	{
		Utility::Core::Node* Node;
		Utility::Core::SlotSide SlotSide;
		size_t SlotIndex;
	};

private:
	virtual void OnGui() override;
	virtual void OnMessage(Service::MessageType type, void* data) override;
	virtual void OnInput(Service::InputMessageType type, const Service::InputMessageData& data) override;

	void Initialize();

	SceneNode& GetSceneNode(Utility::Core::NodeID nodeID);
	void RemoveConnections(Utility::Core::Slot& slot, Utility::Core::SlotSide slotSide);

private:
	const Tool::Window& m_Window;

	std::vector<Function> m_FunctionList;
	size_t m_SelectedFunction;

	ConnectionData m_ConnectionData;
	bool m_IsConnecting = false;
	bool m_IsConnectingCancelled = false;

};

struct SceneNode
{
	Utility::Core::Node Node = Utility::Core::Node(0);
	Gfx::Vector2 Position;
};
struct Function
{
	std::vector<SceneNode> SceneNodes;
	size_t CoreNodeCount = 1;

	Gfx::Vector2 DragOffset;
};

} // namespace Koala::Editor

#endif // KOALA__EDITOR__CODE_BOARD
