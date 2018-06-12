#include <Koala/VirtualMachine/Interpreter.h>
#include <Koala/Utility/Instruction.h>
#include <Koala/Utility/Extra/LogManager.h>
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
	// For easy use
	auto& stack = m_Stack;
	auto& stackFrame = m_StackFrame;

	auto& code = m_Code;
	auto& IP = m_InstructionPtr;

	auto& getStringFromStack = [&stack]() -> std::string
	{
		std::string str;
		while(true)
		{
			char c = stack.Pop8();
			if(c == '\0')
			{
				break;
			}
			str += c;
		}
		return str;
	};

	while(IP < code.size())
	{
		switch(ReadValue<Koala::Utility::Instruction>())
		{
			case Koala::Utility::Instruction::nop:
			{
				break;
			}
			case Koala::Utility::Instruction::halt:
			{
				IP = code.size();

				break;
			}
			case Koala::Utility::Instruction::push8:
			{
				stack.Push8(ReadValue<char>());

				break;
			}
			case Koala::Utility::Instruction::push64:
			{
				stack.Push64(ReadValue<long long>());

				break;
			}
			case Koala::Utility::Instruction::dup8:
			{
				stack.Push8(stack.Top8());

				break;
			}
			case Koala::Utility::Instruction::dup64:
			{
				stack.Push64(stack.Top64i());

				break;
			}
			case Koala::Utility::Instruction::drop8:
			{
				stack.Pop8();

				break;
			}
			case Koala::Utility::Instruction::drop64:
			{
				stack.Pop64i();

				break;
			}
			case Koala::Utility::Instruction::swap8:
			{
				char first = stack.Pop8();
				char second = stack.Pop8();
				stack.Push8(first);
				stack.Push8(second);

				break;
			}
			case Koala::Utility::Instruction::swap64:
			{
				long long first = stack.Pop64i();
				long long second = stack.Pop64i();
				stack.Push64(first);
				stack.Push64(second);

				break;
			}
			case Koala::Utility::Instruction::add8:
			{
				stack.Push8(stack.Pop8() + stack.Pop8());

				break;
			}
			case Koala::Utility::Instruction::add64:
			{
				stack.Push64(stack.Pop64f() + stack.Pop64f());

				break;
			}
			case Koala::Utility::Instruction::add64i:
			{
				stack.Push64(stack.Pop64i() + stack.Pop64i());

				break;
			}
			case Koala::Utility::Instruction::sub8:
			{
				stack.Push8(stack.Pop8() - stack.Pop8());

				break;
			}
			case Koala::Utility::Instruction::sub64:
			{
				stack.Push64(stack.Pop64f() - stack.Pop64f());

				break;
			}
			case Koala::Utility::Instruction::sub64i:
			{
				stack.Push64(stack.Pop64i() - stack.Pop64i());

				break;
			}
			case Koala::Utility::Instruction::mul:
			{
				stack.Push64(stack.Pop64f() * stack.Pop64f());

				break;
			}
			case Koala::Utility::Instruction::div:
			{
				auto first = stack.Pop64f();
				auto second = stack.Pop64f();
				if(second != 0)
				{
					stack.Push64(first / second);
				}
				else
				{
					ExitMessage(Utility::Text::DivideByZeroError);
					IP = code.size();
				}

				break;
			}
			case Koala::Utility::Instruction::mod:
			{
				stack.Push64(std::fmod(stack.Pop64f(), stack.Pop64f()));

				break;
			}
			case Koala::Utility::Instruction::sqrt:
			{
				stack.Push64(std::sqrt(stack.Pop64f()));

				break;
			}
			case Koala::Utility::Instruction::pow:
			{
				stack.Push64(std::pow(stack.Pop64f(), stack.Pop64f()));

				break;
			}
			case Koala::Utility::Instruction::printchar:
			{
				std::printf("%c", stack.Pop8());

				break;
			}
			case Koala::Utility::Instruction::readchar:
			{
				stack.Push8(Utility::Extra::Util::ReadLine()[0]);

				break;
			}
			case Koala::Utility::Instruction::printnum:
			{
				std::printf("%f", stack.Pop64f());

				break;
			}
			case Koala::Utility::Instruction::readnum:
			{
				auto input = Utility::Extra::Util::ReadLine();
				if(('0'<=input[0] && input[0]<='9') || 
					(input.size()>1 && input[0]=='-' && '0'<=input[1] && input[1]<='9'))
				{
					double num = std::stod(input);
					stack.Push64(*((long long*)&num));
				}
				else
				{
					ExitMessage(Utility::Text::NumberRequiredError);
					IP = code.size();
				}

				break;
			}
			case Koala::Utility::Instruction::printstr:
			{
				while(true)
				{
					char c = stack.Pop8();
					if(c == '\0')
						break;

					std::printf("%c", c);
				}

				break;
			}
			case Koala::Utility::Instruction::readstr:
			{
				std::string line = Utility::Extra::Util::ReadLine();
				stack.Push8('\0');
				for( auto rIt=line.rbegin() ; rIt!=line.rend() ; ++rIt )
				{
					stack.Push8(*rIt);
				}

				break;
			}
			case Koala::Utility::Instruction::printref:
			{
				std::printf("0x%p", (void*)stack.Pop64i());

				break;
			}
			case Koala::Utility::Instruction::reserve:
			{
				stackFrame.Reserve(static_cast<size_t>(stack.Pop64i()));

				break;
			}
			case Koala::Utility::Instruction::setlocal:
			{
				char mode = stack.Pop8();
				size_t offset = static_cast<size_t>(stack.Pop64i());
				if(mode == 0) // char
				{
					stackFrame.GetVariableRef8(offset) = stack.Pop8();
				}
				else if(mode == 1) // double
				{
					stackFrame.GetVariableRef64f(offset) = stack.Pop64f();
				}
				else if(mode == 2) // long long
				{
					stackFrame.GetVariableRef64i(offset) = stack.Pop64i();
				}
				else if(mode == 3) // string
				{
					while(true)
					{
						char currentChar = stack.Pop8();
						stackFrame.GetVariableRef8(offset) = currentChar;

						++offset;
						if(currentChar == '\0')
							break;
					}
				}
				else
				{
					ExitMessage(Utility::Text::SetLocalModeError, IP, mode);
					IP = code.size();
				}

				break;
			}
			case Koala::Utility::Instruction::getlocal:
			{
				char mode = stack.Pop8();
				size_t offset = static_cast<size_t>(stack.Pop64i());
				if(mode == 0) // char
				{
					stack.Push8(stackFrame.GetVariableRef8(offset));
				}
				else if(mode == 1) // double
				{
					stack.Push64(stackFrame.GetVariableRef64f(offset));
				}
				else if(mode == 2) // long long
				{
					stack.Push64(stackFrame.GetVariableRef64i(offset));
				}
				else if(mode == 3) // string
				{
					size_t startOffset = offset;
					while(stackFrame.GetVariableRef8(offset) != '\0')
						++offset;

					while(true)
					{
						stack.Push8(stackFrame.GetVariableRef8(offset));

						if(offset == startOffset)
							break;

						--offset;
					}
				}
				else
				{
					ExitMessage(Utility::Text::GetLocalModeError, IP, mode);
					IP = code.size();
				}

				break;
			}
			case Koala::Utility::Instruction::getconst:
			{
				char mode = stack.Pop8();
				size_t offset = static_cast<size_t>(stack.Pop64i());
				if(mode == 0) // char
				{
					stack.Push8(stackFrame.GetConstant8(offset));
				}
				else if(mode == 1) // double
				{
					stack.Push64(stackFrame.GetConstant64f(offset));
				}
				else if(mode == 2) // long long
				{
					stack.Push64(stackFrame.GetConstant64i(offset));
				}
				else if(mode == 3) // string
				{
					size_t startOffset = offset;
					while(stackFrame.GetConstant8(offset) != '\0')
						++offset;

					while(true)
					{
						stack.Push8(stackFrame.GetConstant8(offset));

						if(offset == startOffset)
							break;

						--offset;
					}
				}
				else
				{
					ExitMessage(Utility::Text::GetConstModeError, IP, mode);
					IP = code.size();
				}

				break;
			}
			case Koala::Utility::Instruction::ref:
			{
				size_t offset = static_cast<size_t>(stack.Pop64i());
				stack.Push64((long long)&stackFrame.GetVariableRef8(offset));

				break;
			}
			case Koala::Utility::Instruction::deref:
			{
				char mode = stack.Pop8();
				if(mode == 0) // char
				{
					stack.Push8(*((char*)stack.Pop64i()));
				}
				else if(mode == 1) // double
				{
					stack.Push64(*((double*)stack.Pop64i()));
				}
				else if(mode == 2) // long long
				{
					stack.Push64(*((long long*)stack.Pop64i()));
				}
				else
				{
					ExitMessage(Utility::Text::DerefModeError, IP, mode);
					IP = code.size();
				}

				break;
			}
			case Koala::Utility::Instruction::assignref:
			{
				char mode = stack.Pop8();
				void* memory = (void*)stack.Pop64i();
				if(mode == 0) // char
				{
					*((char*)memory) = stack.Pop8();
				}
				else if(mode == 1) // double
				{
					*((double*)memory) = stack.Pop64f();
				}
				else if(mode == 2) // long long
				{
					*((long long*)memory) = stack.Pop64i();
				}
				else
				{
					ExitMessage(Utility::Text::AssignrefModeError, IP, mode);
					IP = code.size();
				}

				break;
			}
			case Koala::Utility::Instruction::alloc:
			{
				size_t size = static_cast<size_t>(stack.Pop64i());
				void* memory = new char[size];
				if(memory != nullptr)
				{
					stack.Push64((long long)memory);
				}
				else
				{
					ExitMessage(Utility::Text::AllocError, IP, size);
					IP = code.size();
				}

				break;
			}
			case Koala::Utility::Instruction::dealloc:
			{
				void* memory = (void*)stack.Pop64i();
				delete[] memory;

				break;
			}
			case Koala::Utility::Instruction::d2i:
			{
				stack.Push64(static_cast<long long>(stack.Pop64f()));

				break;
			}
			case Koala::Utility::Instruction::i2d:
			{
				stack.Push64(static_cast<double>(stack.Pop64i()));

				break;
			}
			case Koala::Utility::Instruction::s2i:
			{
				std::string str = getStringFromStack();
				if(('0'<=str[0] && str[0]<='9') || 
					(str.size()>1 && str[0]=='-' && '0'<=str[1] && str[1]<='9'))
				{
					stack.Push64(std::stoll(str));
				}
				else
				{
					ExitMessage(Utility::Text::S2IConversionError, IP);
					IP = code.size();
				}

				break;
			}
			case Koala::Utility::Instruction::i2s:
			{
				std::string str = std::to_string(stack.Pop64i());
				stack.Push8('\0');
				for( auto rIt=str.rbegin() ; rIt!=str.rend() ; ++rIt )
				{
					stack.Push8(*rIt);
				}

				break;
			}
			case Koala::Utility::Instruction::s2d:
			{
				std::string str = getStringFromStack();
				if(('0'<=str[0] && str[0]<='9') || 
					(str.size()>1 && str[0]=='-' && '0'<=str[1] && str[1]<='9'))
				{
					stack.Push64(std::stod(str));
				}
				else
				{
					ExitMessage(Utility::Text::S2DConversionError, IP);
					IP = code.size();
				}

				break;
			}
			case Koala::Utility::Instruction::d2s:
			{
				std::string str = std::to_string(stack.Pop64f());
				stack.Push8('\0');
				for( auto rIt=str.rbegin() ; rIt!=str.rend() ; ++rIt )
				{
					stack.Push8(*rIt);
				}

				break;
			}
			case Koala::Utility::Instruction::equal8:
			{
				stack.Push8(stack.Pop8() == stack.Pop8());

				break;
			}
			case Koala::Utility::Instruction::equal64:
			{
				stack.Push8(stack.Pop64i() == stack.Pop64i());

				break;
			}
			case Koala::Utility::Instruction::greater8:
			{
				stack.Push8(stack.Pop8() > stack.Pop8());

				break;
			}
			case Koala::Utility::Instruction::greater64:
			{
				stack.Push8(stack.Pop64i() > stack.Pop64i());

				break;
			}
			case Koala::Utility::Instruction::and:
			{
				stack.Push8(stack.Pop8() && stack.Pop8());

				break;
			}
			case Koala::Utility::Instruction::or:
			{
				stack.Push8(stack.Pop8() || stack.Pop8());

				break;
			}
			case Koala::Utility::Instruction::not:
			{
				stack.Push8(!stack.Pop8());

				break;
			}
			case Koala::Utility::Instruction::equalstr:
			{
				std::string str1 = getStringFromStack();
				std::string str2 = getStringFromStack();

				stack.Push8(str1 == str2);

				break;
			}
			case Koala::Utility::Instruction::jmp:
			{
				IP = static_cast<size_t>(stack.Pop64i());

				break;
			}
			case Koala::Utility::Instruction::jmpcond:
			{
				size_t newIP = static_cast<size_t>(stack.Pop64i());
				bool cond = stack.Pop8();
				if(cond == true)
				{
					IP = newIP;
				}

				break;
			}
			case Koala::Utility::Instruction::call:
			{
				size_t newIP = static_cast<size_t>(stack.Pop64i());
				stackFrame.PushFrame(IP);
				IP = newIP;

				break;
			}
			case Koala::Utility::Instruction::ret:
			{
				IP = stackFrame.PopFrame();

				break;
			}
			default:
			{
				ExitMessage(Utility::Text::InvalidBytecodeError, (unsigned char)code[IP-1], IP);
				IP = code.size();

				break;
			}
		} // switch
		
		// Check stack errors
		if(stack.GetLastError() != Koala::Utility::Text::Empty)
		{
			ExitMessageExtended(Utility::Text::StackError, stack.GetLastError(), IP);
			IP = code.size();
		}
		else if(stackFrame.GetLastError() != Koala::Utility::Text::Empty)
		{
			ExitMessageExtended(Utility::Text::StackFrameError, stackFrame.GetLastError(), IP);
			IP = code.size();
		}

	} // while
}

template<typename T> T Interpreter::ReadValue()
{
	T value = *reinterpret_cast<T*>(&m_Code[m_InstructionPtr]);
	m_InstructionPtr += sizeof(T);

	Utility::Extra::Util::FixEndianness(value);
	return value;
}

} // namespace Koala::VirtualMachine
