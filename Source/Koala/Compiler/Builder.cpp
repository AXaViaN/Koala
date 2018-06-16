#include <Koala/Compiler/Builder.h>
#include <Koala/Utility/File.h>
#include <Koala/Utility/Instruction.h>
#include <Koala/Utility/Extra/Util.h>
#include <algorithm>
#include <stack>

namespace Koala::Compiler {

struct FunctionFlow
{
	std::vector<Utility::Core::Node> Flow;
};
struct FunctionFlows
{
	std::vector<FunctionFlow> Flows;
};

struct TemporaryVariable
{
	Utility::Core::Port Port;
	size_t Position = 0;
	size_t Size = 0;
	unsigned char Mode = 0; // mode0: char, mode1: double, mode2: long long, mode3: string
};
struct FunctionLink
{
	std::string FunctionName;
	size_t LinkPosition = 0;
};
struct BranchLink
{
	size_t BranchPosition = 0;
	size_t LinkPosition = 0;
};
struct LoopInterrupt
{
	Utility::Text Type; // Continue or Break
	size_t LinkPosition = 0;
};
struct LoopInterrupts
{
	std::vector<LoopInterrupt> Interrupts;
};
struct FunctionData
{
	std::string Name;
	std::string Code;
	std::vector<TemporaryVariable> TemporaryVariables;
	std::vector<FunctionLink> FunctionLinks;
	std::vector<BranchLink> BranchLinks;

	std::stack<Utility::Core::NodeID> LoopStack;
	std::map<Utility::Core::NodeID, LoopInterrupts> LoopInterrupts;

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
static void ProcessDefaultFunction(std::string& constantBufferCode, FunctionData& functionData, const std::map<Utility::Core::NodeID, FunctionFlows>& flows, const Utility::Core::Node& node, const Koala::Utility::Text& defaultFunctionText);
static TemporaryVariable CreateLocalVariable(FunctionData& functionData, const Utility::Core::Port& port, const Utility::Core::VariableType& variableType);
static void CallLocalVariable(FunctionData& functionData, const TemporaryVariable& temporaryVariable);
static void LoadLocalVariable(FunctionData& functionData, const TemporaryVariable& temporaryVariable);

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
	}
	
	// Linker
	for( auto& functionData : functionDatas )
	{
		size_t currentFunctionPosition = functionPositions[functionData.Name];

		// Link functions
		for( auto& link : functionData.FunctionLinks )
		{
			size_t linkedFunctionPosition = functionPositions[link.FunctionName];
			auto value = Utility::Extra::Util::GetBinaryNumber<long long>(linkedFunctionPosition);

			auto replaceBegin = code.begin() + currentFunctionPosition + link.LinkPosition;
			code.replace(replaceBegin, replaceBegin+value.size(), value);
		}

		// Link branches
		for( auto& link : functionData.BranchLinks )
		{
			auto fixedLink = link;
			fixedLink.BranchPosition += currentFunctionPosition;
			fixedLink.LinkPosition += currentFunctionPosition;

			auto value = Utility::Extra::Util::GetBinaryNumber<long long>(fixedLink.BranchPosition);
			auto replaceBegin = code.begin() + fixedLink.LinkPosition;
			code.replace(replaceBegin, replaceBegin+value.size(), value);
		}
	}

	// Write code to the binary file
	Utility::File binaryFile(m_BinaryPath);
	if(binaryFile.IsValid())
	{
		binaryFile.DeleteContents();
		binaryFile.Write(Utility::KoalaBinaryMark + code);
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
		auto& nextSlots = currentNode.GetSlots(slotSide);
		if(nextSlots.size() > 0 && 
		   nextSlots.at(0).IsConnected())
		{
			nextNodeID = nextSlots.at(0).GetConnections().at(0).NodeID;
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
					CallLocalVariable(functionData, variable);
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
				ProcessDefaultFunction(constantBufferCode, functionData, flows, node, nodeTemplate.NameText);
			}
		}

		// Output
		if(node.GetFrontSlots().size() > 0 && 
		   nodeTemplate.NameText != Utility::Text::If && 
		   nodeTemplate.NameText != Utility::Text::ForLoop && 
		   nodeTemplate.NameText != Utility::Text::WhileLoop)
		{
			for( auto it=node.GetFrontSlots().begin()+1 ; it!=node.GetFrontSlots().end() ; ++it )
			{
				auto& slot = *it;

				auto variable = CreateLocalVariable(functionData, slot.GetPort(), slot.GetVariable().GetVariableType());
				LoadLocalVariable(functionData, variable);
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
static void ProcessDefaultFunction(std::string& constantBufferCode, FunctionData& functionData, const std::map<Utility::Core::NodeID, FunctionFlows>& flows, const Utility::Core::Node& node, const Koala::Utility::Text& defaultFunctionText)
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
			// Linking data
			BranchLink trueBranch;
			BranchLink endBranch;

			// Jump to true branch if the condition is true
			functionData.Code += (unsigned char)Utility::Instruction::push64;
			trueBranch.LinkPosition = functionData.Code.size();
			functionData.Code += Utility::Extra::Util::GetBinaryNumber<long long>(-1);
			functionData.Code += (unsigned char)Utility::Instruction::jmpcond;

			// Process the false branch
			CompileNodes(constantBufferCode, functionData, flows, flows.at(node.GetID()).Flows[1].Flow, false);

			// Jump to the end-of-branch
			functionData.Code += (unsigned char)Utility::Instruction::push64;
			endBranch.LinkPosition = functionData.Code.size();
			functionData.Code += Utility::Extra::Util::GetBinaryNumber<long long>(-1);
			functionData.Code += (unsigned char)Utility::Instruction::jmp;

			// Process the true branch
			trueBranch.BranchPosition = functionData.Code.size();
			CompileNodes(constantBufferCode, functionData, flows, flows.at(node.GetID()).Flows[0].Flow, false);
			
			// end-of-branch
			endBranch.BranchPosition = functionData.Code.size();

			// Linking data
			functionData.BranchLinks.emplace_back(trueBranch);
			functionData.BranchLinks.emplace_back(endBranch);

			break;
		}
		case Koala::Utility::Text::ForLoop:
		{
			// Push loop node ID
			functionData.LoopStack.push(node.GetID());

			// Linking data
			BranchLink jumpBackLink;
			BranchLink conditionLink;

			// Looping variables
			auto indexVariable = CreateLocalVariable(functionData, node.GetFrontSlots().at(1).GetPort(), Utility::Core::VariableType::Float64);
			auto endVariable = CreateLocalVariable(functionData, {}, Utility::Core::VariableType::Float64);

			// Remove the floating values and save to local variable
			functionData.Code += (unsigned char)Utility::Instruction::d2i;
			functionData.Code += (unsigned char)Utility::Instruction::i2d;
			LoadLocalVariable(functionData, indexVariable);
			functionData.Code += (unsigned char)Utility::Instruction::d2i;
			functionData.Code += (unsigned char)Utility::Instruction::i2d;
			LoadLocalVariable(functionData, endVariable);

			// Comparison
			jumpBackLink.BranchPosition = functionData.Code.size();
			CallLocalVariable(functionData, endVariable);
			CallLocalVariable(functionData, indexVariable);
			functionData.Code += (unsigned char)Utility::Instruction::greater64;

			// Jump to end-of-loop
			functionData.Code += (unsigned char)Utility::Instruction::push64;
			conditionLink.LinkPosition = functionData.Code.size();
			functionData.Code += Utility::Extra::Util::GetBinaryNumber<long long>(-1);
			functionData.Code += (unsigned char)Utility::Instruction::jmpcond;

			// Process the loop body
			CompileNodes(constantBufferCode, functionData, flows, flows.at(node.GetID()).Flows[0].Flow, false);

			// Increase the index
			size_t continueBranchPosition = functionData.Code.size();
			CallLocalVariable(functionData, indexVariable);
			functionData.Code += (unsigned char)Utility::Instruction::push64;
			functionData.Code += Utility::Extra::Util::GetBinaryNumber<double>(1.0);
			functionData.Code += (unsigned char)Utility::Instruction::add64;
			LoadLocalVariable(functionData, indexVariable);

			// Jump back
			functionData.Code += (unsigned char)Utility::Instruction::push64;
			jumpBackLink.LinkPosition = functionData.Code.size();
			functionData.Code += Utility::Extra::Util::GetBinaryNumber<long long>(-1);
			functionData.Code += (unsigned char)Utility::Instruction::jmp;

			// end-of-loop
			conditionLink.BranchPosition = functionData.Code.size();

			// Linking data
			functionData.BranchLinks.emplace_back(jumpBackLink);
			functionData.BranchLinks.emplace_back(conditionLink);

			// Loop interrupt links
			for( auto& interrupt : functionData.LoopInterrupts[node.GetID()].Interrupts )
			{
				BranchLink link;
				link.LinkPosition = interrupt.LinkPosition;

				if(interrupt.Type == Utility::Text::Continue)
				{
					link.BranchPosition = continueBranchPosition;
				}
				else if(interrupt.Type == Utility::Text::Break)
				{
					link.BranchPosition = conditionLink.BranchPosition;
				}

				functionData.BranchLinks.emplace_back(link);
			}

			// Pop loop node ID
			functionData.LoopStack.pop();

			break;
		}
		case Koala::Utility::Text::WhileLoop:
		{
			// Push loop node ID
			functionData.LoopStack.push(node.GetID());

			// Linking data
			BranchLink jumpBackLink;
			BranchLink conditionLink;

			// Looping variable
			TemporaryVariable conditionVariable;
			auto& slot = node.GetBackSlots().at(1);
			if(slot.IsConnected())
			{
				conditionVariable = functionData.GetTemporaryVariable(slot.GetConnections().at(0));
				functionData.Code += (unsigned char)Utility::Instruction::drop8;
			}
			else
			{
				conditionVariable = CreateLocalVariable(functionData, {}, Utility::Core::VariableType::Boolean);
				LoadLocalVariable(functionData, conditionVariable);
			}

			// Condition check
			jumpBackLink.BranchPosition = functionData.Code.size();
			CallLocalVariable(functionData, conditionVariable);
			functionData.Code += (unsigned char)Utility::Instruction::not;

			// Jump to end-of-loop
			functionData.Code += (unsigned char)Utility::Instruction::push64;
			conditionLink.LinkPosition = functionData.Code.size();
			functionData.Code += Utility::Extra::Util::GetBinaryNumber<long long>(-1);
			functionData.Code += (unsigned char)Utility::Instruction::jmpcond;

			// Process the loop body
			CompileNodes(constantBufferCode, functionData, flows, flows.at(node.GetID()).Flows[0].Flow, false);

			// Jump back
			functionData.Code += (unsigned char)Utility::Instruction::push64;
			jumpBackLink.LinkPosition = functionData.Code.size();
			functionData.Code += Utility::Extra::Util::GetBinaryNumber<long long>(-1);
			functionData.Code += (unsigned char)Utility::Instruction::jmp;

			// end-of-loop
			conditionLink.BranchPosition = functionData.Code.size();

			// Linking data
			functionData.BranchLinks.emplace_back(jumpBackLink);
			functionData.BranchLinks.emplace_back(conditionLink);

			// Loop interrupt links
			for( auto& interrupt : functionData.LoopInterrupts[node.GetID()].Interrupts )
			{
				BranchLink link;
				link.LinkPosition = interrupt.LinkPosition;

				if(interrupt.Type == Utility::Text::Continue)
				{
					link.BranchPosition = jumpBackLink.BranchPosition;
				}
				else if(interrupt.Type == Utility::Text::Break)
				{
					link.BranchPosition = conditionLink.BranchPosition;
				}

				functionData.BranchLinks.emplace_back(link);
			}

			// Pop loop node ID
			functionData.LoopStack.pop();

			break;
		}
		case Koala::Utility::Text::Continue:
		{
			LoopInterrupt interrupt;
			interrupt.Type = Utility::Text::Continue;

			functionData.Code += (unsigned char)Utility::Instruction::push64;
			interrupt.LinkPosition = functionData.Code.size();
			functionData.Code += Utility::Extra::Util::GetBinaryNumber<long long>(-1);
			functionData.Code += (unsigned char)Utility::Instruction::jmp;

			if(functionData.LoopStack.empty() == false)
			{
				auto loopNodeID = functionData.LoopStack.top();
				functionData.LoopInterrupts[loopNodeID].Interrupts.emplace_back(interrupt);
			}

			break;
		}
		case Koala::Utility::Text::Break:
		{
			LoopInterrupt interrupt;
			interrupt.Type = Utility::Text::Break;

			functionData.Code += (unsigned char)Utility::Instruction::push64;
			interrupt.LinkPosition = functionData.Code.size();
			functionData.Code += Utility::Extra::Util::GetBinaryNumber<long long>(-1);
			functionData.Code += (unsigned char)Utility::Instruction::jmp;

			if(functionData.LoopStack.empty() == false)
			{
				auto loopNodeID = functionData.LoopStack.top();
				functionData.LoopInterrupts[loopNodeID].Interrupts.emplace_back(interrupt);
			}

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
static TemporaryVariable CreateLocalVariable(FunctionData& functionData, const Utility::Core::Port& port, const Utility::Core::VariableType& variableType)
{
	size_t temporaryVariablePosition = functionData.TemporaryVariables.back().Position + 
									   functionData.TemporaryVariables.back().Size;

	auto& temporaryVariable = functionData.TemporaryVariables.emplace_back();
	temporaryVariable.Port = port;
	temporaryVariable.Position = temporaryVariablePosition;
	switch(variableType)
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

	return temporaryVariable;
}
static void CallLocalVariable(FunctionData& functionData, const TemporaryVariable& temporaryVariable)
{
	functionData.Code += (unsigned char)Utility::Instruction::push64;
	functionData.Code += Utility::Extra::Util::GetBinaryNumber<long long>(temporaryVariable.Position);
	functionData.Code += (unsigned char)Utility::Instruction::push8;
	functionData.Code += temporaryVariable.Mode;
	functionData.Code += (unsigned char)Utility::Instruction::getlocal;
}
static void LoadLocalVariable(FunctionData& functionData, const TemporaryVariable& temporaryVariable)
{
	functionData.Code += (unsigned char)Utility::Instruction::push64;
	functionData.Code += Utility::Extra::Util::GetBinaryNumber<long long>(temporaryVariable.Position);
	functionData.Code += (unsigned char)Utility::Instruction::push8;
	functionData.Code += temporaryVariable.Mode;
	functionData.Code += (unsigned char)Utility::Instruction::setlocal;
}

} // namespace Koala::Compiler
