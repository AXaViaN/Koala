#ifndef KOALA__VIRTUAL_MACHINE__INTERPRETER
#define KOALA__VIRTUAL_MACHINE__INTERPRETER

#include <Koala/VirtualMachine/Tool/Stack.h>
#include <Koala/VirtualMachine/Tool/StackFrame.h>
#include <string>

namespace Koala::VirtualMachine {

class Interpreter
{
public:
	Interpreter(const std::string& bytecode);

	void Run();

	template<typename T> T ReadValue();

private:
	Tool::Stack m_Stack;
	Tool::StackFrame m_StackFrame;

	std::string m_Code;
	size_t m_InstructionPtr = 0;

};

} // namespace Koala::VirtualMachine

#endif // KOALA__VIRTUAL_MACHINE__INTERPRETER
