#include <Koala/Utility/Serialization.h>
#include <Koala/Utility/File.h>

namespace Koala::Utility {

static const Core::SlotSide SlotSides[] = {Core::SlotSide::Back, Core::SlotSide::Front};
static const Core::Variable DummyVariable = Core::Variable("", Core::VariableType::None);

void Serialization::SaveProject(const std::string& path, const Data& data)
{
	// Open an empty file
	File projectFile(path);
	if(projectFile.IsValid() == false)
	{
		return;
	}
	projectFile.DeleteContents();

	// Variable write helper
	auto writeVariable = [&projectFile](const Core::Variable& variable)
	{
		projectFile.WriteLine(variable.m_Name);
		projectFile.WriteBinaryNumber(variable.m_NameText);
		projectFile.WriteBinaryNumber(variable.m_Type);
		projectFile.WriteBinaryNumber(variable.m_ValueFloat64);
		projectFile.WriteBinaryNumber(variable.m_ValueBoolean);
		projectFile.WriteLine(variable.m_ValueString);
	};

	// Write the project mark and name
	projectFile.WriteLine(KoalaProjectMark + data.ProjectName);

	// Current function selection
	projectFile.WriteBinaryNumber(data.SelectedFunction);

	// Functions
	projectFile.WriteBinaryNumber(data.Functions.size());
	for( auto& function : data.Functions )
	{
		// Mouse drag
		projectFile.WriteBinaryNumber(function.DragOffset.X);
		projectFile.WriteBinaryNumber(function.DragOffset.Y);
		projectFile.WriteBinaryNumber(function.CoreNodeCount);

		// Nodes
		projectFile.WriteBinaryNumber(function.SceneNodes.size());
		for( auto& sceneNode : function.SceneNodes )
		{
			// Position
			projectFile.WriteBinaryNumber(sceneNode.Position.X);
			projectFile.WriteBinaryNumber(sceneNode.Position.Y);

			// Node
			projectFile.WriteBinaryNumber(sceneNode.Node.GetID());
			projectFile.WriteBinaryNumber(sceneNode.Node.m_FunctionID);
			for( auto& slotSide : SlotSides )
			{
				projectFile.WriteBinaryNumber(sceneNode.Node.GetSlots(slotSide).size());
				for( auto& slot : sceneNode.Node.GetSlots(slotSide) )
				{
					auto writeSlotPort = [&projectFile](const Core::Port& port)
					{
						projectFile.WriteBinaryNumber(port.NodeID);
						projectFile.WriteBinaryNumber(port.SlotIndex);
					};

					// Current port
					writeSlotPort(slot.GetPort());

					// Connections
					projectFile.WriteBinaryNumber(slot.GetConnectionCount());
					for( auto& connection : slot.GetConnections() )
					{
						writeSlotPort(connection);
					}

					// Variable
					writeVariable(slot.GetVariable());
				}
			}
		}
	}

	// Serialize function manager
	auto& userFunctions = Core::FunctionManager::GetUserFunctions();
	projectFile.WriteBinaryNumber(userFunctions.size());
	for( auto& userFunction : userFunctions )
	{
		projectFile.WriteBinaryNumber(userFunction.ID);
		projectFile.WriteLine(userFunction.Name);
		projectFile.WriteBinaryNumber(userFunction.NameText);

		projectFile.WriteBinaryNumber(userFunction.BackSlots.size());
		for( auto& variable : userFunction.BackSlots )
		{
			writeVariable(variable);
		}
		projectFile.WriteBinaryNumber(userFunction.FrontSlots.size());
		for( auto& variable : userFunction.FrontSlots )
		{
			writeVariable(variable);
		}
	}

	// Serialize ID generators
	projectFile.WriteBinaryNumber(Core::Node::PeekNextID());
	projectFile.WriteBinaryNumber(Core::FunctionManager::PeekNextID());
}
Serialization::Data Serialization::LoadProject(const std::string& path)
{
	// Initialize the data
	Data data;

	// Open existing file
	File projectFile(path, Utility::FileType::Binary, false);
	if(projectFile.IsValid() == false)
	{
		return data;
	}
	projectFile.MoveHeadToFront();

	// Check if the file contains project mark
	if(projectFile.Read(KoalaProjectMark.size()) != KoalaProjectMark)
	{
		return data;
	}

	// Erase current functions
	Core::FunctionManager::RemoveAllUserFunctions();

	// Variable read helper
	auto readVariable = [&projectFile]() -> Core::Variable
	{
		Core::Variable variable = DummyVariable;
		variable.m_Name = projectFile.ReadLine();
		variable.m_NameText = projectFile.ReadBinaryNumber<Text>();
		variable.m_Type = projectFile.ReadBinaryNumber<Core::VariableType>();
		variable.m_ValueFloat64 = projectFile.ReadBinaryNumber<double>();
		variable.m_ValueBoolean = projectFile.ReadBinaryNumber<bool>();
		variable.m_ValueString = projectFile.ReadLine();
		return variable;
	};

	// Read the project name
	data.ProjectName = projectFile.ReadLine();

	// Current function selection
	data.SelectedFunction = projectFile.ReadBinaryNumber<size_t>();

	// Functions
	auto functionCount = projectFile.ReadBinaryNumber<size_t>();
	while(functionCount-- > 0)
	{
		auto& function = data.Functions.emplace_back();

		// Mouse drag
		function.DragOffset.X = projectFile.ReadBinaryNumber<float>();
		function.DragOffset.Y = projectFile.ReadBinaryNumber<float>();
		function.CoreNodeCount = projectFile.ReadBinaryNumber<size_t>();

		// Nodes
		auto sceneNodeCount = projectFile.ReadBinaryNumber<size_t>();
		while(sceneNodeCount-- > 0)
		{
			auto& sceneNode = function.SceneNodes.emplace_back();

			// Position
			sceneNode.Position.X = projectFile.ReadBinaryNumber<float>();
			sceneNode.Position.Y = projectFile.ReadBinaryNumber<float>();

			// Node
			sceneNode.Node.m_ID = projectFile.ReadBinaryNumber<Core::NodeID>();
			sceneNode.Node.m_FunctionID = projectFile.ReadBinaryNumber<Core::FunctionID>();
			for( auto& slotSide : SlotSides )
			{
				auto slotCount = projectFile.ReadBinaryNumber<size_t>();
				while(slotCount-- > 0)
				{
					auto& slot = sceneNode.Node.GetSlots(slotSide).emplace_back(0, 0, DummyVariable);

					auto readSlotPort = [&projectFile]() -> Core::Port
					{
						Core::Port port;
						port.NodeID = projectFile.ReadBinaryNumber<Core::NodeID>();
						port.SlotIndex = projectFile.ReadBinaryNumber<size_t>();
						return port;
					};

					// Current port
					slot.m_Port = readSlotPort();

					// Connections
					auto slotConnectionCount = projectFile.ReadBinaryNumber<size_t>();
					while(slotConnectionCount-- > 0)
					{
						auto& connection = slot.m_Connections.emplace_back();
						connection = readSlotPort();
					}

					// Variable
					slot.m_Variable = readVariable();
				}
			}
		}
	}

	// Deserialize function manager
	auto userFunctionsCount = projectFile.ReadBinaryNumber<size_t>();
	while(userFunctionsCount-- > 0)
	{
		Core::FunctionInfo userFunction;

		userFunction.ID = projectFile.ReadBinaryNumber<Core::FunctionID>();
		userFunction.Name = projectFile.ReadLine();
		userFunction.NameText = projectFile.ReadBinaryNumber<Text>();

		auto backSlotCount = projectFile.ReadBinaryNumber<size_t>();
		while(backSlotCount-- > 0)
		{
			auto& variable = userFunction.BackSlots.emplace_back(DummyVariable);
			variable = readVariable();
		}
		auto frontSlotCount = projectFile.ReadBinaryNumber<size_t>();
		while(frontSlotCount-- > 0)
		{
			auto& variable = userFunction.FrontSlots.emplace_back(DummyVariable);
			variable = readVariable();
		}

		Core::FunctionManager::SerializeAdd(userFunction);
	}

	// Deserialize ID generators
	Core::Node::SetNextID(projectFile.ReadBinaryNumber<size_t>());
	Core::FunctionManager::SetNextID(projectFile.ReadBinaryNumber<size_t>());

	data.IsDataValid = true;
	return data;
}

} // namespace Koala::Utility
