#include <Koala/Compiler/Builder.h>
#include <Koala/Utility/Serialization.h>
#include <Koala/Utility/Extra/LogManager.h>
#include <Koala/Utility/Extra/Util.h>

static const std::vector<Koala::Utility::Serialization::Function> GetFunctions(const std::string& filePath);

int main(int argc, char* argv[])
{
	Koala::Utility::Extra::LogManager::Initialize("Koala Compiler");

	// Check machine requirements
	if(sizeof(double) != 8u || 
	   sizeof(double) != sizeof(long long))
	{
		ExitMessage(Koala::Utility::Text::DataSizeMismatchError);
		return 0;
	}

	// Load functions from project file
	// Select file from argument list if provided
	// Else ask from command line
	std::string filePath;
	if(argc > 1)
	{
		filePath = argv[1];
	}
	else
	{
		std::printf("%s = ", Koala::Utility::Resource::GetText(Koala::Utility::Text::KprojFile).c_str());
		filePath = Koala::Utility::Extra::Util::ReadLine();
		std::printf("\n-------------------------\n\n");
	}
	auto functions = GetFunctions(filePath);

	// Check if the data is valid
	if(functions.size() == 0)
	{
		return 0;
	}

	filePath.erase(filePath.find_last_of('.'));
	filePath += ".koa";

	// Init & run the builder
	Koala::Compiler::Builder(filePath, functions).Run();

	return 0;
}

static const std::vector<Koala::Utility::Serialization::Function> GetFunctions(const std::string& filePath)
{
	auto projectData = Koala::Utility::Serialization::LoadProject(filePath);
	if(projectData.IsDataValid == false)
	{
		ExitMessage(Koala::Utility::Text::InvalidProjectFileError);
		return {};
	}

	return projectData.Functions;
}
