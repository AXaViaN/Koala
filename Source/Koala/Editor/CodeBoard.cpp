#include <Koala/Editor/CodeBoard.h>
#include <Koala/Editor/Private/CodeBoardButtonPanel.h>
#include <Koala/Editor/Gfx/Renderer.h>
#include <Koala/Editor/Tool/Input.h>

namespace Koala::Editor {

static const Gfx::Vector2 FunctionStartPosition = Gfx::Vector2(0.05f, 0.1f);

CodeBoard::CodeBoard(const Tool::Window& window) :
	Gfx::Panel(window, 
			   Gfx::Vector2(CodeBoardStartPosition().GetX(), 
							CodeBoardStartPosition().GetY()+CodeBoardButtonPanel::ButtonHeight), 
			   CodeBoardEndPosition()),

	m_Window(window)
{
	Panel::DisableScrollBar();
	Panel::DisablePanelInput();

	Initialize();

	// Test user functions
#if 0
	m_FunctionList.emplace_back();
	m_FunctionList.back().SceneNodes.emplace_back();
	m_FunctionList.back().SceneNodes.back().Position = FunctionStartPosition;
	{
		Utility::Core::FunctionInfo functionInfo;
		functionInfo.Name = "Test";
		functionInfo.FrontSlots.emplace_back("", Utility::Core::VariableType::None);
		functionInfo.FrontSlots.emplace_back("Deger", Utility::Core::VariableType::Float64);

		size_t programFunctionID = Utility::Core::FunctionManager::Add(functionInfo);
		m_FunctionList.back().SceneNodes.back().Node = Utility::Core::Node(programFunctionID);
	}
#endif
}

void CodeBoard::OnGui()
{
	using namespace Gfx;

	// Draw buttons
	{
		std::vector<std::string> functionNames;
		for( size_t i=0 ; i<m_FunctionList.size() ; ++i )
		{
			functionNames.emplace_back(m_FunctionList[i].SceneNodes[0].Node.GetName());
		}

		CodeBoardButtonPanel::Instance(m_Window).Update(functionNames, m_SelectedFunction);
	}

	// Draw Nodes
	for( auto& sceneNode : m_FunctionList[m_SelectedFunction].SceneNodes )
	{
		Renderer::DrawNode(sceneNode.Node, sceneNode.Position + m_FunctionList[m_SelectedFunction].DragOffset);
	}

	// Draw Connections
	for( auto& sceneNode : m_FunctionList[m_SelectedFunction].SceneNodes )
	{
		Renderer::DrawFrontConnections(sceneNode.Node, sceneNode.Position + m_FunctionList[m_SelectedFunction].DragOffset);
	}

	// Connection drawing & input
	if(m_IsConnecting)
	{
		auto position = GetSceneNode(m_ConnectionData.Node->GetID()).Position;
		position += m_FunctionList[m_SelectedFunction].DragOffset;

		Renderer::DrawConnectionToMouse(*m_ConnectionData.Node, position, m_ConnectionData.SlotSide, m_ConnectionData.SlotIndex);
	}

	if(m_IsConnectingCancelled)
	{
		m_IsConnecting = false;
		m_IsConnectingCancelled = false;
	}
}
void CodeBoard::OnMessage(Service::MessageType type, void* data)
{
	// Node movement and removal commands comes from Renderer::DrawNode

	switch(type)
	{
		case Service::MessageType::NewProject:
		{
			Initialize();
			break;
		}
		case Service::MessageType::SaveProject:
		{
			auto saveData = static_cast<Service::SaveProjectData*>(data);

			saveData->Functions = m_FunctionList;
			saveData->SelectedFunction = m_SelectedFunction;

			break;
		}
		case Service::MessageType::LoadProject:
		{
			auto loadData = static_cast<Service::LoadProjectData*>(data);

			m_FunctionList = loadData->Functions;
			m_SelectedFunction = loadData->SelectedFunction;

			break;
		}
		case Service::MessageType::NodeMove:
		{
			if(m_IsConnecting)
			{
				break;
			}

			auto moveData = static_cast<Service::NodeMoveData*>(data);

			auto& sceneNode = GetSceneNode(moveData->Node.GetID());
			sceneNode.Position += {moveData->RawMouseDelta.GetX() / m_Window.GetWidth(), 
								   moveData->RawMouseDelta.GetY() / m_Window.GetHeight()};

			break;
		}
		case Service::MessageType::NodeRemove:
		{
			if(m_IsConnecting)
			{
				break;
			}

			auto removeData = static_cast<Service::NodeRemoveData*>(data);
			auto& sceneNodes = m_FunctionList[m_SelectedFunction].SceneNodes;
			size_t removeIndex = 0;
			for( size_t i=0 ; i<sceneNodes.size() ; ++i )
			{
				if(&sceneNodes[i].Node == &removeData->Node)
				{
					removeIndex = i;
					break;
				}
			}

			if(removeIndex >= m_FunctionList[m_SelectedFunction].CoreNodeCount)
			{
				auto& node = sceneNodes[removeIndex].Node;
				// First remove the connections
				static Utility::Core::SlotSide slotSides[] = {Utility::Core::SlotSide::Front, 
															  Utility::Core::SlotSide::Back};
				for( auto& slotSide : slotSides )
				{
					for( auto& slot : node.GetSlots(slotSide) )
					{
						if(slot.IsConnected())
						{
							RemoveConnections(slot, slotSide);
						}
					}
				}

				// Remove the node
				sceneNodes.erase(sceneNodes.begin() + removeIndex);
			}

			break;
		}
		case Service::MessageType::ConnectionBegin:
		{
			if(m_IsConnecting)
			{
				break;
			}

			auto connectionData = static_cast<Service::ConnectionBeginData*>(data);
			auto& sceneNode = GetSceneNode(connectionData->Node.GetID());
			auto& slot = sceneNode.Node.GetSlots(connectionData->SlotSide).at(connectionData->SlotIndex);
			if(slot.IsConnected() &&
			   (connectionData->SlotSide == Utility::Core::SlotSide::Back || 
				slot.GetVariable().GetVariableType() == Utility::Core::VariableType::None))
			{
				break;
			}

			m_ConnectionData.Node = &sceneNode.Node;
			m_ConnectionData.SlotSide = connectionData->SlotSide;
			m_ConnectionData.SlotIndex = connectionData->SlotIndex;

			m_IsConnecting = true;
			break;
		}
		case Service::MessageType::ConnectionEnd:
		{
			if(m_IsConnecting == false)
			{
				break;
			}

			auto otherConnectionData = static_cast<Service::ConnectionEndData*>(data);

			auto& slot0 = m_ConnectionData.Node->GetSlots(m_ConnectionData.SlotSide).at(m_ConnectionData.SlotIndex);
			auto& slot1 = otherConnectionData->Node.GetSlots(otherConnectionData->SlotSide).at(otherConnectionData->SlotIndex);
			if(&otherConnectionData->Node == m_ConnectionData.Node || 
			   otherConnectionData->SlotSide == m_ConnectionData.SlotSide || 
			   slot0.GetVariable().GetVariableType() != slot1.GetVariable().GetVariableType())
			{
				m_IsConnecting = false;
				break;
			}

			// Flow connections can't be multiple
			if(slot1.GetVariable().GetVariableType() == Utility::Core::VariableType::None && 
			   slot1.IsConnected())
			{
				m_IsConnecting = false;
				break;
			}

			// Back connections can't be multiple
			Utility::Core::Slot* backSlot;
			if(m_ConnectionData.SlotSide == Utility::Core::SlotSide::Back)
			{
				backSlot = &slot0;
			}
			else
			{
				backSlot = &slot1;
			}
			if(backSlot->IsConnected())
			{
				m_IsConnecting = false;
				break;
			}

			slot0.Connect(slot1);

			m_IsConnecting = false;
			break;
		}
		case Service::MessageType::ConnectionCancel:
		{
			m_IsConnectingCancelled = true;
			break;
		}
		case Service::MessageType::ConnectionRemove:
		{
			if(m_IsConnecting)
			{
				break;
			}

			auto connectionData = static_cast<Service::ConnectionRemoveData*>(data);
			
			auto& slot = connectionData->Node.GetSlots(connectionData->SlotSide)[connectionData->SlotIndex];
			RemoveConnections(slot, connectionData->SlotSide);

			break;
		}
		case Service::MessageType::RequestNode:
		{
			auto requestData = static_cast<Service::RequestNodeData*>(data);
			auto& sceneNode = GetSceneNode(requestData->NodeID);

			requestData->Node = &sceneNode.Node;
			requestData->NodePosition = sceneNode.Position + m_FunctionList[m_SelectedFunction].DragOffset;
			
			break;
		}
		case Service::MessageType::SpawnNode:
		{
			auto spawnData = static_cast<Service::SpawnNodeData*>(data);

			auto node = Utility::Core::Node(spawnData->FunctionID);
			if(node.GetID() == 0)
			{
				break;
			}

			auto& currentSceneNodes = m_FunctionList[m_SelectedFunction].SceneNodes;
			currentSceneNodes.emplace_back();
			currentSceneNodes.back().Node = node;
			currentSceneNodes.back().Position = Gfx::Vector2(0.35f, 0.35f) - 
												m_FunctionList[m_SelectedFunction].DragOffset;
			
			break;
		}
	}
}
void CodeBoard::OnInput(Service::InputMessageType type, const Service::InputMessageData& data)
{
	static Gfx::Vector2 oldMousePosition;

	switch(type)
	{
		case Koala::Editor::Service::InputMessageType::MousePress:
		{
			if(data.Button == Tool::MouseButtonType::Left)
			{
				oldMousePosition = Tool::Input::GetMousePosition();
			}
			break;
		}
		case Koala::Editor::Service::InputMessageType::MouseHold:
		{
			if(data.Button == Tool::MouseButtonType::Left)
			{
				auto mouseDelta = Tool::Input::GetMousePosition() - oldMousePosition;
				oldMousePosition = Tool::Input::GetMousePosition();

				m_FunctionList[m_SelectedFunction].DragOffset += mouseDelta;
			}
			break;
		}
	}
}

void CodeBoard::Initialize()
{
	m_FunctionList.clear();

	m_FunctionList.emplace_back();
	m_FunctionList.back().SceneNodes.emplace_back();
	{
		auto& programFunctionInfo = Utility::Core::FunctionManager::GetDefault(Utility::Text::Program);

		m_FunctionList.back().SceneNodes.back().Node = Utility::Core::Node(programFunctionInfo.ID);
		m_FunctionList.back().SceneNodes.back().Position = FunctionStartPosition;
	}

	m_SelectedFunction = 0;
}

SceneNode& CodeBoard::GetSceneNode(Utility::Core::NodeID nodeID)
{
	for( auto& sceneNode : m_FunctionList[m_SelectedFunction].SceneNodes )
	{
		if(sceneNode.Node.GetID() == nodeID)
		{
			return sceneNode;
		}
	}

	return m_FunctionList[m_SelectedFunction].SceneNodes[0];
}
void CodeBoard::RemoveConnections(Utility::Core::Slot& slot, Utility::Core::SlotSide slotSide)
{
	if(slot.IsConnected())
	{
		Utility::Core::SlotSide otherSide = slotSide == Utility::Core::SlotSide::Front ? 
											Utility::Core::SlotSide::Back : Utility::Core::SlotSide::Front;
		
		for( auto& connection : slot.GetConnections() )
		{
			auto& other = GetSceneNode(connection.NodeID);
			other.Node.GetSlots(otherSide)[connection.SlotIndex].Disconnect(slot.GetPort());
		}

		slot.DisconnectAll();
	}
}

} // namespace Koala::Editor
