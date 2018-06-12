#include <Koala/Compiler/Builder.h>
#include <Koala/Utility/File.h>
#include <Koala/Utility/Instruction.h>
#include <Koala/Utility/Extra/Util.h>
#include <algorithm>

namespace Koala::Compiler {

static constexpr size_t NumberVariableSize = 8u;
static constexpr size_t StringVariableSize = 512u;

static std::vector<Utility::Core::Node> GetNodes(Koala::Utility::Serialization::Function function);
static void ProcessConstant(std::string& constantBufferCode, std::string& functionCode, const Koala::Utility::Core::Variable& variable);
static void ProcessDefaultFunction(std::string& functionCode, const Koala::Utility::Text& defaultFunctionText);

Builder::Builder(const std::string& binaryPath, const std::vector<Koala::Utility::Serialization::Function>& functions) : 
	m_BinaryPath(binaryPath),
	m_Functions(functions)
{

}

void Builder::Run()
{
	struct FunctionLink
	{
		std::string FunctionName;
		size_t LinkPosition = 0;
	};
	struct FunctionData
	{
		std::string Name;
		std::string Code;
		std::vector<FunctionLink> FunctionLinks;
	};

	// Code to write
	std::string constantBufferCode;
	std::vector<FunctionData> functionDatas;

	// Start compiling
	for( auto& function : m_Functions )
	{
		auto& functionData = functionDatas.emplace_back();

		auto nodes = GetNodes(function);
		functionData.Name = nodes[0].GetName();

		auto& startNode = nodes.front();
		auto& returnNode = nodes.back();
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

			// Return node only outputs the variables
			if(&node == &returnNode)
			{
				functionData.Code += (unsigned char)Utility::Instruction::ret;
				break;
			}

			// Produce instruction
			auto& nodeTemplate = Utility::Core::FunctionManager::Get(node.GetFunctionID());
			// Start node doesn't have an instruction
			if(&node != &startNode && 
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
					ProcessDefaultFunction(functionData.Code, nodeTemplate.NameText);
				}
			}

			// Output
			for( auto& slot : node.GetFrontSlots() )
			{

			}
		}
	}

	// Create code
	std::string code;
	std::map<std::string, size_t> functionPositions;

	code += Utility::Extra::Util::GetBinaryNumber<long long>(constantBufferCode.size());
	code += constantBufferCode;
	for( auto& functionData : functionDatas )
	{
		functionPositions[functionData.Name] = code.size();
		code += functionData.Code;
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

static std::vector<Utility::Core::Node> GetNodes(Koala::Utility::Serialization::Function function)
{
	// Setup searching
	Utility::Core::NodeID nextNodeID;
	Utility::Core::SlotSide slotSide;
	if(function.CoreNodeCount == 1)
	{
		nextNodeID = function.SceneNodes.at(0).Node.GetFrontSlots().at(0).GetConnections().at(0).NodeID;
		slotSide = Utility::Core::SlotSide::Front;
	}
	else
	{
		nextNodeID = function.SceneNodes.at(1).Node.GetID();
		// If there is a return node, reverse search
		slotSide = Utility::Core::SlotSide::Back;
	}

	// Search and fill
	std::vector<Utility::Core::Node> nodes;
	while(nextNodeID != 0)
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

static void ProcessDefaultFunction(std::string& functionCode, const Koala::Utility::Text& defaultFunctionText)
{
	switch(defaultFunctionText)
	{
		case Koala::Utility::Text::Add:
		{
			functionCode += (unsigned char)Utility::Instruction::add64;
			break;
		}
		case Koala::Utility::Text::Subtract:
		{
			functionCode += (unsigned char)Utility::Instruction::sub64;
			break;
		}
		case Koala::Utility::Text::Multiply:
		{
			functionCode += (unsigned char)Utility::Instruction::mul;
			break;
		}
		case Koala::Utility::Text::Divide:
		{
			functionCode += (unsigned char)Utility::Instruction::div;
			break;
		}
		case Koala::Utility::Text::SquareRoot:
		{
			functionCode += (unsigned char)Utility::Instruction::sqrt;
			break;
		}
		case Koala::Utility::Text::Power:
		{
			functionCode += (unsigned char)Utility::Instruction::pow;
			break;
		}
		case Koala::Utility::Text::Modulo:
		{
			functionCode += (unsigned char)Utility::Instruction::mod;
			break;
		}
		case Koala::Utility::Text::Print:
		{
			functionCode += (unsigned char)Utility::Instruction::printstr;
			functionCode += (unsigned char)Utility::Instruction::push8;
			functionCode += '\n';
			functionCode += (unsigned char)Utility::Instruction::printchar;
			break;
		}
		case Koala::Utility::Text::ReadNumber:
		{
			functionCode += (unsigned char)Utility::Instruction::readnum;
			break;
		}
		case Koala::Utility::Text::ReadString:
		{
			functionCode += (unsigned char)Utility::Instruction::readstr;
			break;
		}
		case Koala::Utility::Text::If:
		{
			// TODO: Solve branching
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
			functionCode += (unsigned char)Utility::Instruction::greater64;
			break;
		}
		case Koala::Utility::Text::Smaller:
		{
			functionCode += (unsigned char)Utility::Instruction::swap64;
			functionCode += (unsigned char)Utility::Instruction::greater64;
			break;
		}
		case Koala::Utility::Text::And:
		{
			functionCode += (unsigned char)Utility::Instruction::and;
			break;
		}
		case Koala::Utility::Text::Or:
		{
			functionCode += (unsigned char)Utility::Instruction::or;
			break;
		}
		case Koala::Utility::Text::NumberEquals:
		{
			functionCode += (unsigned char)Utility::Instruction::equal64;
			break;
		}
		case Koala::Utility::Text::StringEquals:
		{
			functionCode += (unsigned char)Utility::Instruction::equalstr;
			break;
		}
		case Koala::Utility::Text::NumberToString:
		{
			functionCode += (unsigned char)Utility::Instruction::d2s;
			break;
		}
		case Koala::Utility::Text::StringToNumber:
		{
			functionCode += (unsigned char)Utility::Instruction::s2d;
			break;
		}
	}
}

} // namespace Koala::Compiler
