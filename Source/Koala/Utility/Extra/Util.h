#ifndef KOALA__UTILITY__EXTRA__UTIL
#define KOALA__UTILITY__EXTRA__UTIL

#include <string>

namespace Koala::Utility::Extra {

class Util
{
public:
	static std::string ReadLine();

	template<typename T> static void FixEndianness(T& value)
	{
		FixEndianness(&value, sizeof(T));
	}
	static void FixEndianness(void* value, size_t size);

	template<typename T> static std::string GetBinaryNumber(const T& value)
	{
		return GetBinaryNumber(&value, sizeof(T));
	}
	static std::string GetBinaryNumber(const void* value, size_t size);

};

} // namespace Koala::Utility::Extra

#endif // KOALA__UTILITY__EXTRA__UTIL
