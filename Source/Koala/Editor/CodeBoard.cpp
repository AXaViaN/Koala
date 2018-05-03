#include <Koala/Editor/CodeBoard.h>
#include <Koala/Editor/Private/CodeBoardButtonPanel.h>
#include <Koala/Editor/Gfx/Renderer.h>
#include <Koala/Editor/Tool/Input.h>

namespace Koala::Editor {

static void SetupDefaultFunctions(Utility::Core::Node& programNode);

CodeBoard::CodeBoard(const Tool::Window& window) :
	Gfx::Panel(window, 
			   Gfx::Vector2(CodeBoardStartPosition().GetX(), 
							CodeBoardStartPosition().GetY()+CodeBoardButtonPanel::ButtonHeight), 
			   CodeBoardEndPosition()),

	m_Window(window)
{
	Panel::DisableScrollBar();
	Panel::DisablePanelInput();

	m_FunctionList.emplace_back();
	m_FunctionList.back().SceneNodes.emplace_back();
	m_FunctionList.back().SceneNodes.back().Position = Gfx::Vector2(0.05f, 0.1f);

	// Test nodes
#if 1
	m_FunctionList.back().SceneNodes.emplace_back();
	m_FunctionList.back().SceneNodes.back().Position = Gfx::Vector2(0.5f, 0.5f);
	{
		Utility::Core::FunctionInfo functionInfo;
		functionInfo.Name = "Topla";
		functionInfo.BackSlots.emplace_back("", Utility::Core::VariableType::None);
		functionInfo.BackSlots.emplace_back("X", Utility::Core::VariableType::Float64);
		functionInfo.BackSlots.emplace_back("Y", Utility::Core::VariableType::Float64);
		functionInfo.FrontSlots.emplace_back("", Utility::Core::VariableType::None);
		functionInfo.FrontSlots.emplace_back("Sonuc", Utility::Core::VariableType::Float64);

		size_t programFunctionID = Utility::Core::FunctionManager::Add(functionInfo);
		m_FunctionList.back().SceneNodes.back().Node = Utility::Core::Node(programFunctionID);
	}
	m_FunctionList.back().SceneNodes.emplace_back();
	m_FunctionList.back().SceneNodes.back().Position = Gfx::Vector2(0.25f, 0.75f);
	{
		Utility::Core::FunctionInfo functionInfo;
		functionInfo.Name = "Sayi Oku";
		functionInfo.BackSlots.emplace_back("", Utility::Core::VariableType::None);
		functionInfo.FrontSlots.emplace_back("", Utility::Core::VariableType::None);
		functionInfo.FrontSlots.emplace_back("Sayi", Utility::Core::VariableType::Float64);

		size_t programFunctionID = Utility::Core::FunctionManager::Add(functionInfo);
		m_FunctionList.back().SceneNodes.back().Node = Utility::Core::Node(programFunctionID);
	}

	m_FunctionList.emplace_back();
	m_FunctionList.back().SceneNodes.emplace_back();
	m_FunctionList.back().SceneNodes.back().Position = Gfx::Vector2(0.7f, 0.3f);
	{
		Utility::Core::FunctionInfo functionInfo;
		functionInfo.Name = "Test";
		functionInfo.FrontSlots.emplace_back("", Utility::Core::VariableType::None);
		functionInfo.FrontSlots.emplace_back("Deger", Utility::Core::VariableType::Float64);

		size_t programFunctionID = Utility::Core::FunctionManager::Add(functionInfo);
		m_FunctionList.back().SceneNodes.back().Node = Utility::Core::Node(programFunctionID);
	}
	m_FunctionList.back().SceneNodes.emplace_back();
	m_FunctionList.back().SceneNodes.back().Position = Gfx::Vector2(0.2f, 0.7f);
	{
		Utility::Core::FunctionInfo functionInfo;
		functionInfo.Name = "Yazdir";
		functionInfo.BackSlots.emplace_back("", Utility::Core::VariableType::None);
		functionInfo.BackSlots.emplace_back("", Utility::Core::VariableType::String);
		functionInfo.FrontSlots.emplace_back("", Utility::Core::VariableType::None);

		size_t programFunctionID = Utility::Core::FunctionManager::Add(functionInfo);
		m_FunctionList.back().SceneNodes.back().Node = Utility::Core::Node(programFunctionID);
	}
#endif

	SetupDefaultFunctions(m_FunctionList[0].SceneNodes[0].Node);

	// Test connections
#if 1
	m_FunctionList[0].SceneNodes[0].Node.GetFrontSlots()[0].Connect(m_FunctionList[0].SceneNodes[1].Node.GetBackSlots()[0].GetPort());
	m_FunctionList[0].SceneNodes[1].Node.GetBackSlots()[0].Connect(m_FunctionList[0].SceneNodes[0].Node.GetFrontSlots()[0].GetPort());

	m_FunctionList[1].SceneNodes[0].Node.GetFrontSlots()[0].Connect(m_FunctionList[1].SceneNodes[1].Node.GetBackSlots()[0].GetPort());
	m_FunctionList[1].SceneNodes[1].Node.GetBackSlots()[0].Connect(m_FunctionList[1].SceneNodes[0].Node.GetFrontSlots()[0].GetPort());
	m_FunctionList[1].SceneNodes[0].Node.GetFrontSlots()[1].Connect(m_FunctionList[1].SceneNodes[1].Node.GetBackSlots()[1].GetPort());
	m_FunctionList[1].SceneNodes[1].Node.GetBackSlots()[1].Connect(m_FunctionList[1].SceneNodes[0].Node.GetFrontSlots()[1].GetPort());
#endif

	m_SelectedFunction = 0;
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
			auto connectionData = static_cast<Service::ConnectionEndData*>(data);
			if(&connectionData->Node == m_ConnectionData.Node)
			{
				m_IsConnecting = false;
				return;
			}

			// TODO: Implement proper connection
			
			if(m_IsConnecting)
			{
				if(m_ConnectionData.SlotSide == Utility::Core::SlotSide::Front)
				{
					m_ConnectionData.Node->GetFrontSlots()[m_ConnectionData.SlotIndex].Connect(connectionData->Node.GetBackSlots()[connectionData->SlotIndex].GetPort());
					connectionData->Node.GetBackSlots()[connectionData->SlotIndex].Connect(m_ConnectionData.Node->GetFrontSlots()[m_ConnectionData.SlotIndex].GetPort());
				}
				else
				{
					m_ConnectionData.Node->GetBackSlots()[m_ConnectionData.SlotIndex].Connect(connectionData->Node.GetFrontSlots()[connectionData->SlotIndex].GetPort());
					connectionData->Node.GetFrontSlots()[connectionData->SlotIndex].Connect(m_ConnectionData.Node->GetBackSlots()[m_ConnectionData.SlotIndex].GetPort());
				}
			}

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

CodeBoard::SceneNode& CodeBoard::GetSceneNode(Utility::Core::NodeID nodeID)
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

static void SetupDefaultFunctions(Utility::Core::Node& programNode)
{
	// Program node
	{
		Utility::Core::FunctionInfo functionInfo;
		functionInfo.NameText = Utility::Text::Program;
		functionInfo.FrontSlots.emplace_back("", Utility::Core::VariableType::None);

		size_t programFunctionID = Utility::Core::FunctionManager::Add(functionInfo);
		programNode = Utility::Core::Node(programFunctionID);
	}
}

} // namespace Koala::Editor
