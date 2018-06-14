#include <Koala/Compiler/Builder.h>
#include <Koala/Utility/File.h>
#include <Koala/Utility/Instruction.h>
#include <Koala/Utility/Extra/Util.h>
#include <algorithm>

namespace Koala::Compiler {

struct FunctionFlow
{
	std::vector<Utility::Core::Node> Flow;
};
struct FunctionFlows
{
	std::vector<FunctionFlow> Flows;
};

struct FunctionLink
{
	std::string FunctionName;
	size_t LinkPosition = 0;
};
struct TemporaryVariable
{
	Utility::Core::Port Port;
	size_t Position = 0;
	size_t Size = 0;
	unsigned char Mode = 0; // mode0: char, mode1: double, mode2: long long, mode3: string
};
struct FunctionData
{
	std::string Name;
	std::string Code;
	std::vector<FunctionLink> FunctionLinks;
	std::vector<TemporaryVariable> TemporaryVariables;

public:
	FunctionData()
	{
		// For convenience
		TemporaryVariables.emplace_back();
	}

	TemporaryVariable& GetTemporaryVariable(const Utility::Core::Port& port)
	{
		for( auto& variable : TemporaryVariables )
		{
			if(variable.Port == port)
			{
				return variable;
			}
		}

		static TemporaryVariable dummy;
		return dummy;
	}

};

static constexpr size_t NumberVariableSize = 8u;
static constexpr size_t StringVariableSize = 512u;

static std::map<Utility::Core::NodeID, FunctionFlows> GetFlows(const Koala::Utility::Serialization::Function& function);
static void FindFlows(std::map<Utility::Core::NodeID, FunctionFlows>& nodeFlows, const Koala::Utility::Serialization::Function& function, Utility::Core::NodeID flowOwnerID, Utility::Core::NodeID nextNodeID, Utility::Core::SlotSide slotSide);

static void CompileNodes(std::string& constantBufferCode, FunctionData& functionData, const std::map<Utility::Core::NodeID, FunctionFlows>& flows, const std::vector<Utility::Core::Node>& nodes, bool isMainFlow);
static void ProcessConstant(std::string& constantBufferCode, std::string& functionCode, const Koala::Utility::Core::Variable& variable);
static void ProcessDefaultFunction(std::string& constantBufferCode, FunctionData& functionData, const std::map<Utility::Core::NodeID, FunctionFlows>& flows, const Utility::Core::NodeID& nodeID, const Koala::Utility::Text& defaultFunctionText);

Builder::Builder(const std::string& binaryPath, const std::vector<Koala::Utility::Serialization::Function>& functions) : 
	m_BinaryPath(binaryPath),
	m_Functions(functions)
{

}

void Builder::Run()
{
	// Code to write
	std::string constantBufferCode;
	std::vector<FunctionData> functionDatas;

	// Start compiling
	for( auto& function : m_Functions )
	{
		auto& functionData = functionDatas.emplace_back();

		auto flows = GetFlows(function);
		auto& nodes = flows[function.SceneNodes.at(0).Node.GetID()].Flows.at(0).Flow;
		functionData.Name = nodes[0].GetName();

		CompileNodes(constantBufferCode, functionData, flows, nodes, true);
	}

	// Create code
	std::string code;
	std::map<std::string, size_t> functionPositions;

	code += Utility::KoalaBinaryMark;
	code += Utility::Extra::Util::GetBinaryNumber<long long>(constantBufferCode.size());
	code += constantBufferCode;
	for( auto& functionData : functionDatas )
	{
		// Reserve for temporary variables
		size_t temporaryBufferSize = functionData.TemporaryVariables.back().Position + 
									 functionData.TemporaryVariables.back().Size;
		code += (unsigned char)Utility::Instruction::push64;
		code += Utility::Extra::Util::GetBinaryNumber<long long>(temporaryBufferSize);
		code += (unsigned char)Utility::Instruction::reserve;

		functionPositions[functionData.Name] = code.size();
		code += functionData.Code;

		if(functionData.Name == Utility::Resource::GetText(Utility::Text::Program))
		{
			code += (unsigned char)Utility::Instruction::readchar;
		}
	}
	
	// Link functions
	for( auto& functionData : functionDatas )
	{
		size_t currentFunctionPosition = functionPositions[functionData.Name];
		for( auto& link : functionData.FunctionLinks )
		{
			size_t linkedFunctionPosition = functionPositions[link.FunctionName];
			auto value = Utility::Extra::Util::GetBinaryNumber<long long>(linkedFunctionPosition);

			auto linkCodeBegin = code.begin() + currentFunctionPosition + link.LinkPosition;
			code.replace(linkCodeBegin, linkCodeBegin+value.size(), value);
		}
	}

	// Write code to the binary file
	Utility::File binaryFile(m_BinaryPath);
	if(binaryFile.IsValid())
	{
		binaryFile.DeleteContents();
		binaryFile.Write(code);
	}
}

static std::map<Utility::Core::NodeID, FunctionFlows> GetFlows(const Koala::Utility::Serialization::Function& function)
{
	std::map<Utility::Core::NodeID, FunctionFlows> nodeFlows;
	
	// Setup searching
	Utility::Core::NodeID nextNodeID;
	Utility::Core::SlotSide slotSide;
	if(function.CoreNodeCount == 1)
	{
		nextNodeID = function.SceneNodes.at(0).Node.GetID();
		slotSide = Utility::Core::SlotSide::Front;
	}
	else
	{
		nextNodeID = function.SceneNodes.at(1).Node.GetID();
		// If there is a return node, reverse search
		slotSide = Utility::Core::SlotSide::Back;
	}

	// Search and fill flows
	FindFlows(nodeFlows, function, function.SceneNodes.at(0).Node.GetID(), nextNodeID, slotSide);
	
	// Last adjustments
	auto& nodes = nodeFlows[function.SceneNodes.at(0).Node.GetID()].Flows.at(0).Flow;
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

	return nodeFlows;
}
static void FindFlows(std::map<Utility::Core::NodeID, FunctionFlows>& nodeFlows, const Koala::Utility::Serialization::Function& function, Utility::Core::NodeID flowOwnerID, Utility::Core::NodeID nextNodeID, Utility::Core::SlotSide slotSide)
{
	auto& nodes = nodeFlows[flowOwnerID].Flows.emplace_back().Flow;
	while(nextNodeID != 0)
	{
		// Search for nextNodeID
		for( auto& sceneNode : function.SceneNodes )
		{
			if(sceneNode.Node.GetID() == nextNodeID)
			{
				nodes.emplace_back(sceneNode.Node);
				break;
			}
		}

		// Handle branching flows
		auto& currentNode = nodes.back();
		auto& currentNodeTemplate = Utility::Core::FunctionManager::Get(currentNode.GetFunctionID());
		if(currentNodeTemplate.NameText == Utility::Text::If)
		{
			// True flow
			auto& trueSlot = currentNode.GetFrontSlots().at(1);
			Utility::Core::NodeID trueNextNodeID = 0;
			if(trueSlot.IsConnected())
			{
				trueNextNodeID = trueSlot.GetConnections().at(0).NodeID;
			}
			FindFlows(nodeFlows, function, currentNode.GetID(), trueNextNodeID, Utility::Core::SlotSide::Front);

			// False flow
			auto& falseSlot = currentNode.GetFrontSlots().at(2);
			Utility::Core::NodeID falseNextNodeID = 0;
			if(falseSlot.IsConnected())
			{
				falseNextNodeID = falseSlot.GetConnections().at(0).NodeID;
			}
			FindFlows(nodeFlows, function, currentNode.GetID(), falseNextNodeID, Utility::Core::SlotSide::Front);
		}
		else if(currentNodeTemplate.NameText == Utility::Text::ForLoop)
		{
			auto& loopSlot = currentNode.GetFrontSlots().at(2);
			Utility::Core::NodeID loopNextNodeID = 0;
			if(loopSlot.IsConnected())
			{
				loopNextNodeID = loopSlot.GetConnections().at(0).NodeID;
			}
			FindFlows(nodeFlows, function, currentNode.GetID(), loopNextNodeID, Utility::Core::SlotSide::Front);
		}
		else if(currentNodeTemplate.NameText == Utility::Text::WhileLoop)
		{
			auto& loopSlot = currentNode.GetFrontSlots().at(1);
			Utility::Core::NodeID loopNextNodeID = 0;
			if(loopSlot.IsConnected())
			{
				loopNextNodeID = loopSlot.GetConnections().at(0).NodeID;
			}
			FindFlows(nodeFlows, function, currentNode.GetID(), loopNextNodeID, Utility::Core::SlotSide::Front);
		}

		// Determine the new nextNodeID
		if(currentNode.GetSlots(slotSide).at(0).IsConnected())
		{
			nextNodeID = currentNode.GetSlots(slotSide).at(0).GetConnections().at(0).NodeID;
		}
		else
		{
			nextNodeID = 0;
		}
	}
}

static void CompileNodes(std::string& constantBufferCode, FunctionData& functionData, const std::map<Utility::Core::NodeID, FunctionFlows>& flows, const std::vector<Utility::Core::Node>& nodes, bool isMainFlow)
{
	auto startNodePtr = &nodes.front();
	auto returnNodePtr = &nodes.back();
	if(isMainFlow == false)
	{
		startNodePtr = nullptr;
		returnNodePtr = nullptr;
	}

	for( auto& node : nodes )
	{
		if(node.GetID() == 0)
		{
			functionData.Code += (unsigned char)Utility::Instruction::ret;
			break;
		}

		// Get inputs
		if(node.GetBackSlots().size() > 0)
		{
			for( auto it=node.GetBackSlots().rbegin() ; it!=node.GetBackSlots().rend()-1 ; ++it )
			{
				auto& slot = *it;

				if(slot.IsConnected())
				{
					auto& variable = functionData.GetTemporaryVariable(slot.GetConnections().at(0));

					functionData.Code += (unsigned char)Utility::Instruction::push64;
					functionData.Code += Utility::Extra::Util::GetBinaryNumber<long long>(variable.Position);
					functionData.Code += (unsigned char)Utility::Instruction::push8;
					functionData.Code += variable.Mode;
					functionData.Code += (unsigned char)Utility::Instruction::getlocal;
				}
				else
				{
					ProcessConstant(constantBufferCode, functionData.Code, slot.GetVariable());
				}
			}
		}

		// Return node only outputs the variables
		if(&node == returnNodePtr)
		{
			functionData.Code += (unsigned char)Utility::Instruction::ret;
			break;
		}

		// Produce instruction
		auto& nodeTemplate = Utility::Core::FunctionManager::Get(node.GetFunctionID());
		// Start node doesn't have an instruction
		if(&node != startNodePtr && 
		   nodeTemplate.ID != 0)
		{
			if(nodeTemplate.NameText == Utility::Text::Empty)
			{
				// Call user function
				FunctionLink link;
				link.FunctionName = nodeTemplate.Name;

				functionData.Code += (unsigned char)Utility::Instruction::push64;

				link.LinkPosition = functionData.Code.size();
				functionData.Code += Utility::Extra::Util::GetBinaryNumber<long long>(-1);

				functionData.Code += (unsigned char)Utility::Instruction::call;

				functionData.FunctionLinks.emplace_back(link);
			}
			else
			{
				// Process default function
				ProcessDefaultFunction(constantBufferCode, functionData, flows, node.GetID(), nodeTemplate.NameText);
			}
		}

		// Output
		if(node.GetFrontSlots().size() > 0)
		{
			for( auto it=node.GetFrontSlots().begin()+1 ; it!=node.GetFrontSlots().end() ; ++it )
			{
				auto& slot = *it;

				size_t temporaryVariablePosition = functionData.TemporaryVariables.back().Position + 
					functionData.TemporaryVariables.back().Size;

				auto& variable = slot.GetVariable();
				auto& temporaryVariable = functionData.TemporaryVariables.emplace_back();
				temporaryVariable.Port = slot.GetPort();
				temporaryVariable.Position = temporaryVariablePosition;
				switch(variable.GetVariableType())
				{
					case Koala::Utility::Core::VariableType::Float64:
					{
						temporaryVariable.Size = NumberVariableSize;
						temporaryVariable.Mode = 1;
						break;
					}
					case Koala::Utility::Core::VariableType::String:
					{
						temporaryVariable.Size = StringVariableSize;
						temporaryVariable.Mode = 3;
						break;
					}
					case Koala::Utility::Core::VariableType::Boolean:
					{
						temporaryVariable.Size = 1u;
						temporaryVariable.Mode = 0;
						break;
					}
				}

				functionData.Code += (unsigned char)Utility::Instruction::push64;
				functionData.Code += Utility::Extra::Util::GetBinaryNumber<long long>(temporaryVariable.Position);
				functionData.Code += (unsigned char)Utility::Instruction::push8;
				functionData.Code += temporaryVariable.Mode;
				functionData.Code += (unsigned char)Utility::Instruction::setlocal;
			}
		}
	}
}
static void ProcessConstant(std::string& constantBufferCode, std::string& functionCode, const Koala::Utility::Core::Variable& variable)
{
	size_t constantPosition = constantBufferCode.size();
	unsigned char variableMode = 0;
	switch(variable.GetVariableType())
	{
		case Koala::Utility::Core::VariableType::Float64:
		{
			constantBufferCode += Utility::Extra::Util::GetBinaryNumber(variable.GetValueFloat64());
			variableMode = 1;
			break;
		}
		case Koala::Utility::Core::VariableType::String:
		{
			constantBufferCode += variable.GetValueString().substr(0, variable.GetValueString().find_first_of('\0'));
			constantBufferCode += '\0';
			variableMode = 3;
			break;
		}
		case Koala::Utility::Core::VariableType::Boolean:
		{
			constantBufferCode += variable.GetValueBoolean();
			variableMode = 0;
			break;
		}
	}

	functionCode += (unsigned char)Utility::Instruction::push64;
	functionCode += Utility::Extra::Util::GetBinaryNumber<long long>(constantPosition);
	functionCode += (unsigned char)Utility::Instruction::push8;
	functionCode += variableMode;
	functionCode += (unsigned char)Utility::Instruction::getconst;
}
static void ProcessDefaultFunction(std::string& constantBufferCode, FunctionData& functionData, const std::map<Utility::Core::NodeID, FunctionFlows>& flows, const Utility::Core::NodeID& nodeID, const Koala::Utility::Text& defaultFunctionText)
{
	switch(defaultFunctionText)
	{
		case Koala::Utility::Text::Add:
		{
			functionData.Code += (unsigned char)Utility::Instruction::add64;
			break;
		}
		case Koala::Utility::Text::Subtract:
		{
			functionData.Code += (unsigned char)Utility::Instruction::sub64;
			break;
		}
		case Koala::Utility::Text::Multiply:
		{
			functionData.Code += (unsigned char)Utility::Instruction::mul;
			break;
		}
		case Koala::Utility::Text::Divide:
		{
			functionData.Code += (unsigned char)Utility::Instruction::div;
			break;
		}
		case Koala::Utility::Text::SquareRoot:
		{
			functionData.Code += (unsigned char)Utility::Instruction::sqrt;
			break;
		}
		case Koala::Utility::Text::Power:
		{
			functionData.Code += (unsigned char)Utility::Instruction::pow;
			break;
		}
		case Koala::Utility::Text::Modulo:
		{
			functionData.Code += (unsigned char)Utility::Instruction::mod;
			break;
		}
		case Koala::Utility::Text::Print:
		{
			functionData.Code += (unsigned char)Utility::Instruction::printstr;
			functionData.Code += (unsigned char)Utility::Instruction::push8;
			functionData.Code += '\n';
			functionData.Code += (unsigned char)Utility::Instruction::printchar;
			break;
		}
		case Koala::Utility::Text::ReadNumber:
		{
			functionData.Code += (unsigned char)Utility::Instruction::readnum;
			break;
		}
		case Koala::Utility::Text::ReadString:
		{
			functionData.Code += (unsigned char)Utility::Instruction::readstr;
			break;
		}
		case Koala::Utility::Text::If:
		{
			// TODO: Solve branching

			// Solution:
			// Use CompileNodes() to compile branches
			// Get branch nodes from "flows" and "nodeID"
			//  e.g. CompileNodes(constantBufferCode, functionData, flows, flows.at(nodeID).Flows[0].Flow, false);

			// TODO: Solve jump linking
			break;
		}
		case Koala::Utility::Text::ForLoop:
		{
			// TODO: Solve branching
			break;
		}
		case Koala::Utility::Text::WhileLoop:
		{
			// TODO: Solve branching
			break;
		}
		case Koala::Utility::Text::Greater:
		{
			functionData.Code += (unsigned char)Utility::Instruction::greater64;
			break;
		}
		case Koala::Utility::Text::Smaller:
		{
			functionData.Code += (unsigned char)Utility::Instruction::swap64;
			functionData.Code += (unsigned char)Utility::Instruction::greater64;
			break;
		}
		case Koala::Utility::Text::And:
		{
			functionData.Code += (unsigned char)Utility::Instruction::and;
			break;
		}
		case Koala::Utility::Text::Or:
		{
			functionData.Code += (unsigned char)Utility::Instruction::or;
			break;
		}
		case Koala::Utility::Text::NumberEquals:
		{
			functionData.Code += (unsigned char)Utility::Instruction::equal64;
			break;
		}
		case Koala::Utility::Text::StringEquals:
		{
			functionData.Code += (unsigned char)Utility::Instruction::equalstr;
			break;
		}
		case Koala::Utility::Text::NumberToString:
		{
			functionData.Code += (unsigned char)Utility::Instruction::d2s;
			break;
		}
		case Koala::Utility::Text::StringToNumber:
		{
			functionData.Code += (unsigned char)Utility::Instruction::s2d;
			break;
		}
	}
}

} // namespace Koala::Compiler
