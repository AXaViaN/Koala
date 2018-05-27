#include <Koala/VirtualMachine/Interpreter.h>
#include <Koala/Utility/Instruction.h>
#include <Koala/Utility/Extra/Util.h>

namespace Koala::VirtualMachine {

static constexpr size_t MEGABYTE = 1024u * 1024u;
static constexpr size_t STACK_SIZE = MEGABYTE;
static constexpr size_t STACK_FRAME_SIZE = MEGABYTE;

Interpreter::Interpreter(const std::string& bytecode) : 
	m_Stack(STACK_SIZE), 
	m_StackFrame(STACK_FRAME_SIZE),
	m_Code(bytecode)
{
	// Setup const buffer
	const size_t constBufferSize = (size_t)ReadValue<long long>();

	m_StackFrame.Reserve(constBufferSize);
	for( size_t i=0u ; i<constBufferSize ; ++i )
	{
		m_StackFrame.GetVariableRef8(i) = m_Code[m_InstructionPtr + i];
	}
	m_InstructionPtr += constBufferSize;

	// New frame for main function (returning IP is code.size() so return from main will halt)
	m_StackFrame.PushFrame(m_Code.size());
}

void Interpreter::Run()
{
	
}

template<typename T> T Interpreter::ReadValue()
{
	T value = *reinterpret_cast<T*>(&m_Code[m_InstructionPtr]);
	m_InstructionPtr += sizeof(T);

	Utility::Extra::Util::FixEndianness(value);
	return value;
}

} // namespace Koala::VirtualMachine
