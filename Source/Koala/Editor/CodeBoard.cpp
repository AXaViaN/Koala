#include <Koala/Editor/CodeBoard.h>
#include <Koala/Editor/Private/CodeBoardButtonPanel.h>
#include <Koala/Editor/Gfx/Renderer.h>
#include <Koala/Editor/Tool/Input.h>

namespace Koala::Editor {

static const Gfx::Vector2 FunctionStartPosition = Gfx::Vector2(0.05f, 0.1f);
static const Gfx::Vector2 FunctionReturnPosition = Gfx::Vector2(0.65f, 0.1f);

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
			RemoveSceneNode(removeData->Node);

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
		case Service::MessageType::EditUserFunction:
		{
			auto userFunctionData = static_cast<Service::EditUserFunctionData*>(data);
			auto& userFunction = Utility::Core::FunctionManager::Get(userFunctionData->FunctionID);
			
			// Validity check
			if(userFunction.ID == 0 || 
			   userFunction.Name.size() == 0 || 
			   userFunction.Name[0] == '\0')
			{
				break;
			}

			// Find the function on m_FunctionList
			size_t functionIndex = -1;
			for( size_t i=0 ; i<m_FunctionList.size() ; ++i )
			{
				if(m_FunctionList[i].FunctionID == userFunction.ID)
				{
					functionIndex = i;
					break;
				}
			}

			// Add if the function is not found
			if(functionIndex == -1)
			{
				auto& newFunction = m_FunctionList.emplace_back();
				newFunction.FunctionID = userFunction.ID;
				auto& newFunctionStartNode = newFunction.SceneNodes.emplace_back();
				newFunctionStartNode.Position = FunctionStartPosition;
				{
					Utility::Core::FunctionInfo startFunctionInfo;
					startFunctionInfo.Name = userFunction.Name;
					startFunctionInfo.FrontSlots = userFunction.BackSlots;

					size_t startFunctionID = Utility::Core::FunctionManager::Add(startFunctionInfo);
					newFunctionStartNode.Node = Utility::Core::Node(startFunctionID);
				}

				functionIndex = m_FunctionList.size() - 1u;
			}

			// Check return nodes and update
			if(m_FunctionList[functionIndex].CoreNodeCount == 1 && 
			   userFunction.FrontSlots.size() > 1)
			{
				// Add a return node
				auto& sceneNodes = m_FunctionList[functionIndex].SceneNodes;
				auto& functionReturnNode = *sceneNodes.emplace(sceneNodes.begin()+1);

				functionReturnNode.Position = FunctionReturnPosition;
				{
					Utility::Core::FunctionInfo returnFunctionInfo;
					returnFunctionInfo.Name = userFunction.Name;
					returnFunctionInfo.BackSlots = userFunction.FrontSlots;

					size_t returnFunctionID = Utility::Core::FunctionManager::Add(returnFunctionInfo);
					functionReturnNode.Node = Utility::Core::Node(returnFunctionID);
				}

				m_FunctionList[functionIndex].CoreNodeCount = 2;
			}
			else if(m_FunctionList[functionIndex].CoreNodeCount == 2 && 
					userFunction.FrontSlots.size() == 1)
			{
				// Remove the return node
				auto& sceneNodes = m_FunctionList[functionIndex].SceneNodes;
				auto& functionReturnNode = sceneNodes[1].Node;

				for( auto& slot : functionReturnNode.GetBackSlots() )
				{
					RemoveConnections(slot, Utility::Core::SlotSide::Back);
				}
				sceneNodes.erase(sceneNodes.begin() + 1);
				
				// TODO: Fix name disappearing bug on function remove
				// Utility::Core::FunctionManager::Remove(functionReturnNode.GetFunctionID());

				m_FunctionList[functionIndex].CoreNodeCount = 1;
			}

			// Update the start/return nodes
			{
				auto& functionStartNode = m_FunctionList[functionIndex].SceneNodes[0].Node;
				auto startNodeInfo = Utility::Core::FunctionManager::Get(functionStartNode.GetFunctionID());

				// We only care about the name here
				startNodeInfo.Name = userFunction.Name;
				Utility::Core::FunctionManager::Edit(startNodeInfo.ID, startNodeInfo);

				// Update
				UpdateNodeVariables(functionStartNode, Utility::Core::SlotSide::Front, userFunction.BackSlots);
			}
			if(m_FunctionList[functionIndex].CoreNodeCount == 2)
			{
				auto& functionReturnNode = m_FunctionList[functionIndex].SceneNodes[1].Node;
				auto startNodeInfo = Utility::Core::FunctionManager::Get(functionReturnNode.GetFunctionID());

				// We only care about the name here
				startNodeInfo.Name = userFunction.Name;
				Utility::Core::FunctionManager::Edit(startNodeInfo.ID, startNodeInfo);

				// Update
				UpdateNodeVariables(functionReturnNode, Utility::Core::SlotSide::Back, userFunction.FrontSlots);
			}

			// Check the nodes on all functions and update
			for( auto& function : m_FunctionList )
			{
				for( auto& sceneNode : function.SceneNodes )
				{
					if(sceneNode.Node.GetFunctionID() == userFunction.ID)
					{
						UpdateNodeVariables(sceneNode.Node, Utility::Core::SlotSide::Front, userFunction.FrontSlots);
						UpdateNodeVariables(sceneNode.Node, Utility::Core::SlotSide::Back, userFunction.BackSlots);
					}
				}
			}

			break;
		}
		case Service::MessageType::RemoveUserFunction:
		{
			auto userFunctionData = static_cast<Service::RemoveUserFunctionData*>(data);

			// Remove function from m_FunctionList
			for( size_t i=0 ; i<m_FunctionList.size() ; ++i )
			{
				if(m_FunctionList[i].FunctionID == userFunctionData->FunctionID)
				{
					m_FunctionList.erase(m_FunctionList.begin() + i);
					break;
				}
			}

			// Remove function from SceneNodes
			for( auto& function : m_FunctionList )
			{
				for( auto& sceneNode : function.SceneNodes )
				{
					if(sceneNode.Node.GetFunctionID() == userFunctionData->FunctionID)
					{
						RemoveSceneNode(sceneNode.Node);
					}
				}
			}

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

	auto& newFunction = m_FunctionList.emplace_back();
	auto& newFunctionStartNode = newFunction.SceneNodes.emplace_back();
	newFunctionStartNode.Position = FunctionStartPosition;
	{
		auto& programFunctionInfo = Utility::Core::FunctionManager::GetDefault(Utility::Text::Program);

		newFunction.FunctionID = programFunctionInfo.ID;
		newFunctionStartNode.Node = Utility::Core::Node(programFunctionInfo.ID);
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
void CodeBoard::RemoveSceneNode(const Utility::Core::Node& node)
{
	for( auto& function : m_FunctionList )
	{
		// Find the node
		auto& sceneNodes = function.SceneNodes;
		size_t removeIndex = -1;
		for( size_t i=0 ; i<sceneNodes.size() ; ++i )
		{
			if(&sceneNodes[i].Node == &node)
			{
				removeIndex = i;
				break;
			}
		}
		if(removeIndex == -1)
		{
			continue;
		}

		// Remove
		if(removeIndex >= function.CoreNodeCount)
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
void CodeBoard::UpdateNodeVariables(Utility::Core::Node& node, Utility::Core::SlotSide slotSide, const std::vector<Utility::Core::Variable>& defaultVariables)
{
	// Fix the slot number
	auto& functionStartNodeSlots = node.GetSlots(slotSide);
	if(functionStartNodeSlots.size() > defaultVariables.size())
	{
		size_t eraseIndex = defaultVariables.size();
		size_t lastEraseIndex = functionStartNodeSlots.size();
		for( size_t i=eraseIndex ; i<lastEraseIndex ; ++i )
		{
			RemoveConnections(functionStartNodeSlots[eraseIndex], Utility::Core::SlotSide::Front);
			functionStartNodeSlots.erase(functionStartNodeSlots.begin() + eraseIndex);
		}
	}
	else if(functionStartNodeSlots.size() < defaultVariables.size())
	{
		for( size_t i=functionStartNodeSlots.size() ; i<defaultVariables.size() ; ++i )
		{
			functionStartNodeSlots.emplace_back(node.GetID(), 
												i, 
												defaultVariables[i]);
		}
	}

	// Fix the variable types
	for( size_t i=0 ; i<functionStartNodeSlots.size() ; ++i )
	{
		auto& variable = functionStartNodeSlots[i].GetVariable();
		auto& templateVariable = defaultVariables[i];

		variable.SetName(templateVariable.GetName());
		if(variable.GetVariableType() != templateVariable.GetVariableType())
		{
			variable = templateVariable;
		}
	}
}

} // namespace Koala::Editor
