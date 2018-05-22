#include <Koala/VirtualMachine/Tool/Util.h>

namespace Koala::VirtualMachine::Tool {

std::string Util::ReadLine()
{
	std::string line;
	line.reserve(1u);

	while(true)
	{
		char c;
		std::fread(&c, sizeof(char), 1, stdin);
		if(c == '\n')
			break;

		line += c;
	}

	return line;
}

} // namespace Koala::VirtualMachine::Tool
