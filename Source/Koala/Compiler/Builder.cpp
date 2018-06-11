#include <Koala/Compiler/Builder.h>
#include <Koala/Utility/Instruction.h>
#include <Koala/Utility/Extra/Util.h>
#include <algorithm>

namespace Koala::Compiler {

static constexpr size_t NumberVariableSize = 8u;
static constexpr size_t StringVariableSize = 512u;

static std::vector<Utility::Core::Node> GetNodes(Koala::Utility::Serialization::Function function);
static void ProcessConstant(std::string& constantBufferCode, std::string& functionCode, const Koala::Utility::Core::Variable& variable);

Builder::Builder(const std::string& binaryPath, const std::vector<Koala::Utility::Serialization::Function>& functions) : 
	m_BinaryPath(binaryPath),
	m_Functions(functions)
{

}

void Builder::Run()
{
	struct FunctionData
	{
		std::string Code;
	};

	// Code to write
	std::string constantBufferCode;
	std::vector<FunctionData> functionDatas;

	// Start compiling
	for( auto& function : m_Functions )
	{
		auto& functionData = functionDatas.emplace_back();

		auto nodes = GetNodes(function);
		for( auto& node : nodes )
		{
			if(node.GetID() == 0)
			{
				break;
			}

			// Get inputs
			for( auto it=node.GetBackSlots().rbegin() ; it!=node.GetBackSlots().rend()-1 ; ++it )
			{
				auto& slot = *it;
				
				if(slot.IsConnected())
				{

				}
				else
				{
					ProcessConstant(constantBufferCode, functionData.Code, slot.GetVariable());
				}
			}

			// Produce instruction
			auto& nodeTemplate = Utility::Core::FunctionManager::Get(node.GetFunctionID());
			if(nodeTemplate.ID != 0)
			{
				if(nodeTemplate.NameText == Utility::Text::Empty)
				{
					// Call user function
				}
				else
				{
					// Process default function
				}
			}

			// Output
			for( auto& slot : node.GetFrontSlots() )
			{

			}
		}
	}

	// Link functions

	// Write to the code file
	
}

static std::vector<Utility::Core::Node> GetNodes(Koala::Utility::Serialization::Function function)
{
	// Setup searching
	Utility::Core::NodeID nextNodeID;
	Utility::Core::NodeID endNodeID;
	Utility::Core::SlotSide slotSide;
	if(function.CoreNodeCount == 1)
	{
		nextNodeID = function.SceneNodes.at(0).Node.GetFrontSlots().at(0).GetConnections().at(0).NodeID;
		endNodeID = 0;
		slotSide = Utility::Core::SlotSide::Front;
	}
	else
	{
		nextNodeID = function.SceneNodes.at(1).Node.GetID();
		endNodeID = function.SceneNodes.at(0).Node.GetID();
		// If there is a return node, reverse search
		slotSide = Utility::Core::SlotSide::Back;
	}

	// Search and fill
	std::vector<Utility::Core::Node> nodes;
	while(nextNodeID != endNodeID &&
		  nextNodeID != 0)
	{
		for( auto& sceneNode : function.SceneNodes )
		{
			if(sceneNode.Node.GetID() == nextNodeID)
			{
				nodes.emplace_back(sceneNode.Node);
				break;
			}
		}

		if(nodes.back().GetSlots(slotSide).at(0).IsConnected())
		{
			nextNodeID = nodes.back().GetSlots(slotSide).at(0).GetConnections().at(0).NodeID;
		}
		else
		{
			nextNodeID = 0;
		}
	}

	// Last adjustments
	if(slotSide == Utility::Core::SlotSide::Back)
	{
		// Reverse back
		std::reverse(nodes.begin(), nodes.end());
	}
	else
	{
		// Add a blank return node
		nodes.emplace_back(0);
	}

	return nodes;
}

static void ProcessConstant(std::string& constantBufferCode, std::string& functionCode, const Koala::Utility::Core::Variable& variable)
{
	size_t constantPosition = constantBufferCode.size();
	unsigned char variableMode = 0;
	switch(variable.GetVariableType())
	{
		case Koala::Utility::Core::VariableType::Float64:
			constantBufferCode += Utility::Extra::Util::GetBinaryNumber(variable.GetValueFloat64());
			variableMode = 1;
			break;
		case Koala::Utility::Core::VariableType::String:
			constantBufferCode += variable.GetValueString().substr(0, variable.GetValueString().find_first_of('\0'));
			constantBufferCode += '\0';
			variableMode = 3;
			break;
		case Koala::Utility::Core::VariableType::Boolean:
			constantBufferCode += variable.GetValueBoolean();
			variableMode = 0;
			break;
	}

	functionCode += (unsigned char)Utility::Instruction::push64;
	functionCode += Utility::Extra::Util::GetBinaryNumber<long long>(constantPosition);
	functionCode += (unsigned char)Utility::Instruction::push8;
	functionCode += variableMode;
	functionCode += (unsigned char)Utility::Instruction::getconst;
}

} // namespace Koala::Compiler
