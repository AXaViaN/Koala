#ifndef KOALA__COMPILER__BUILDER
#define KOALA__COMPILER__BUILDER

#include <Koala/Utility/Serialization.h>

namespace Koala::Compiler {

class Builder
{
public:
	Builder(const std::string& binaryPath, const std::vector<Koala::Utility::Serialization::Function>& functions);

	void Run();

private:
	const std::string& m_BinaryPath;
	const std::vector<Koala::Utility::Serialization::Function>& m_Functions;

};

} // namespace Koala::Compiler

#endif // KOALA__COMPILER__BUILDER
