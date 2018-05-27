#include <Koala/Utility/Extra/Util.h>

namespace Koala::Utility::Extra {

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

void Util::FixEndianness(void* value, size_t size)
{
	// Machine byte order flag. Check with a lambda function and save on static const
	static const bool IsLittleEndianMachine = ([]() -> bool 
	{
		int x = 1;

		// If the machine is little-endian, the least important bit
		// will be on lower address. Little-endian will set the x=1
		// as 01000000, big-endian 00000001. So casting to char will
		// result in '1' for little-endian and '0' for big-endian.
		if(*((char*)&x) == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	})();

	if(IsLittleEndianMachine)
	{
		return;
	}

	size_t i = 0;
	size_t j = size - 1u;
	while(i < j)
	{
		std::swap(static_cast<char*>(value)[i], static_cast<char*>(value)[j]);

		++i;
		--j;
	}
}

} // namespace Koala::Utility::Extra
