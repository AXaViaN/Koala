#include <Koala/VirtualMachine/Interpreter.h>
#include <Koala/Utility/File.h>
#include <Koala/Utility/Serialization.h>
#include <Koala/Utility/Extra/LogManager.h>
#include <Koala/Utility/Extra/Util.h>

static const std::string GetCode(const std::string& filePath);

int main(int argc, char* argv[])
{
	Koala::Utility::Extra::LogManager::Initialize("Koala VirtualMachine");

	// Check machine requirements
	if(sizeof(double) != 8u || 
	   sizeof(double) != sizeof(long long))
	{
		ExitMessage(Koala::Utility::Text::DataSizeMismatchError);
		return 0;
	}

	// Get instructions from file
	// Select file from argument list if provided
	// Else ask from command line
	std::string code;
	if(argc > 1)
	{
		code = GetCode(argv[1]);
	}
	else
	{
		std::printf("%s = ", Koala::Utility::Resource::GetText(Koala::Utility::Text::KoaFile).c_str());
		std::string filePath = Koala::Utility::Extra::Util::ReadLine();
		std::printf("\n-------------------------\n\n");

		code = GetCode(filePath);
	}

	// Check if the code is valid
	if(code.size() == 0u)
	{
		return 0;
	}

	// Init & run the bytecode interpreter
	Koala::VirtualMachine::Interpreter(code).Run();

	return 0;
}

static const std::string GetCode(const std::string& filePath)
{
	Koala::Utility::File codeFile(filePath, Koala::Utility::FileType::Binary, false);
	if(codeFile.IsValid() == false)
	{
		ExitMessage(Koala::Utility::Text::FileCouldNotBeFound);
		return "";
	}
	codeFile.MoveHeadToFront();

	// Check if the file contains binary mark
	if(codeFile.Read(Koala::Utility::KoalaBinaryMark.size()) != Koala::Utility::KoalaBinaryMark)
	{
		ExitMessage(Koala::Utility::Text::InvalidCodeFileError);
		return "";
	}

	// Read the code
	std::string code;
	while(true)
	{
		code += codeFile.Read(1u);
		if(codeFile.IsEOF())
		{
			code.pop_back();
			break;
		}
	}

	return code;
}
