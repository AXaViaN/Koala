#ifndef KOALA__VIRTUAL_MACHINE__TOOL__STACK_FRAME
#define KOALA__VIRTUAL_MACHINE__TOOL__STACK_FRAME

#include <Koala/Utility/Resource.h>

namespace Koala::VirtualMachine::Tool {

class StackFrame
{
public:
	StackFrame(size_t size) : 
		m_Memory(new char[size]), m_MemorySize(size)
	{ }
	~StackFrame()
	{
		if(m_Memory)
		{
			delete[] m_Memory;
			m_Memory = nullptr;
			m_MemorySize = 0;
		}
	}

	/**
	*	Save current frame and create a new one
	*	
	*	@param instructionPtr To be saved for returning to same point later
	*/
	void PushFrame(size_t instructionPtr)
	{
		if(m_Marker+2u*sizeof(size_t) > m_MemorySize)
		{
			m_LastError = Koala::Utility::Text::PushFrameStackOverflow;
			return;
		}

		*((size_t*)&m_Memory[m_Marker]) = instructionPtr;
		m_Marker += sizeof(size_t);

		*((size_t*)&m_Memory[m_Marker]) = m_CurrentFrame;
		m_Marker += sizeof(size_t);

		m_CurrentFrame = m_Marker;
	}
	/**
	*	Reserve more data space on current frame
	*	
	*	@param size Data size in bytes
	*/
	void Reserve(size_t size)
	{
		if(m_Marker+size > m_MemorySize)
		{
			m_LastError = Koala::Utility::Text::ReserveStackOverflow;
			return;
		}

		m_Marker += size;
	}
	/**
	*	Remove current frame and load the old one
	*	
	*	@returns Last frame's instruction pointer
	*/
	size_t PopFrame()
	{
		if(m_CurrentFrame == 0)
		{
			m_LastError = Koala::Utility::Text::PopFrameOnFirstFrame;
			return SIZE_MAX;
		}

		m_Marker = m_CurrentFrame - sizeof(size_t);
		m_CurrentFrame = *((size_t*)&m_Memory[m_Marker]);
		m_Marker -= sizeof(size_t);

		return *((size_t*)&m_Memory[m_Marker]);
	}

	char& GetVariableRef8(size_t offset)
	{
		if(m_CurrentFrame+offset+sizeof(char) > m_MemorySize)
		{
			m_LastError = Koala::Utility::Text::GetVariableRef8OutOfBound;
			return m_Memory[0];
		}

		return m_Memory[m_CurrentFrame + offset];
	}
	long long& GetVariableRef64i(size_t offset)
	{
		if(m_CurrentFrame+offset+sizeof(long long) > m_MemorySize)
		{
			m_LastError = Koala::Utility::Text::GetVariableRef64iOutOfBound;
			return *((long long*)&m_Memory[0]);
		}

		return *((long long*)&m_Memory[m_CurrentFrame + offset]);
	}
	double& GetVariableRef64f(size_t offset)
	{
		if(m_CurrentFrame+offset+sizeof(double) > m_MemorySize)
		{
			m_LastError = Koala::Utility::Text::GetVariableRef64fOutOfBound;
			return *((double*)&m_Memory[0]);
		}

		return *((double*)&m_Memory[m_CurrentFrame + offset]);
	}

	char GetConstant8(size_t offset)
	{
		if(offset+sizeof(char) > m_MemorySize)
		{
			m_LastError = Koala::Utility::Text::GetConstant8OutOfBound;
			return 0;
		}

		return m_Memory[offset];
	}
	long long GetConstant64i(size_t offset)
	{
		if(offset+sizeof(long long) > m_MemorySize)
		{
			m_LastError = Koala::Utility::Text::GetConstant64iOutOfBound;
			return 0;
		}

		return *((long long*)&m_Memory[offset]);
	}
	double GetConstant64f(size_t offset)
	{
		if(offset+sizeof(double) > m_MemorySize)
		{
			m_LastError = Koala::Utility::Text::GetConstant64fOutOfBound;
			return 0;
		}

		return *((double*)&m_Memory[offset]);
	}

	/**
	*	Get the last error message (if happend)
	*	
	*	@returns the last error message - nullptr if no error occured
	*/
	Koala::Utility::Text GetLastError()
	{
		return m_LastError;
	}

private:
	char* m_Memory;
	size_t m_MemorySize = 0;
	size_t m_Marker = 0;
	size_t m_CurrentFrame = 0;

	Koala::Utility::Text m_LastError = Koala::Utility::Text::Empty;

};

} // namespace Koala::VirtualMachine::Tool

#endif // KOALA__VIRTUAL_MACHINE__TOOL__STACK_FRAME
