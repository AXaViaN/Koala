#ifndef KOALA__VIRTUAL_MACHINE__INTERPRETER
#define KOALA__VIRTUAL_MACHINE__INTERPRETER

#include <string>

namespace Koala::VirtualMachine {

class Interpreter
{
public:
	Interpreter(const std::string& bytecode);

	void Run();

};

} // namespace Koala::VirtualMachine

#endif // KOALA__VIRTUAL_MACHINE__INTERPRETER
