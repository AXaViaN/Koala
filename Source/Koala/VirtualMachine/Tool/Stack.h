#ifndef KOALA__VIRTUAL_MACHINE__TOOL__STACK
#define KOALA__VIRTUAL_MACHINE__TOOL__STACK

#include <Koala/Utility/Resource.h>

namespace Koala::VirtualMachine::Tool {

class Stack
{
public:
	Stack(size_t size) : 
		m_Memory(new char[size]), m_MemorySize(size)
	{ }
	~Stack()
	{
		if(m_Memory)
		{
			delete[] m_Memory;
			m_Memory = nullptr;
			m_MemorySize = 0;
		}
	}

	void Push8(char data)
	{
		if(m_Marker+DATASIZE8 > m_MemorySize)
		{
			m_LastError = Koala::Utility::Text::Push8StackOverflow;
			return;
		}

		m_Memory[m_Marker] = data;
		m_Marker += DATASIZE8;
	}
	void Push64(long long data)
	{
		if(m_Marker+DATASIZE64 > m_MemorySize)
		{
			m_LastError = Koala::Utility::Text::Push64iStackOverflow;
			return;
		}

		*((long long*)&m_Memory[m_Marker]) = data;
		m_Marker += DATASIZE64;
	}
	void Push64(double data)
	{
		if(m_Marker+DATASIZE64 > m_MemorySize)
		{
			m_LastError = Koala::Utility::Text::Push64fStackOverflow;
			return;
		}

		*((double*)&m_Memory[m_Marker]) = data;
		m_Marker += DATASIZE64;
	}

	char Pop8()
	{
		if(m_Marker < DATASIZE8)
		{
			m_LastError = Koala::Utility::Text::Pop8OnEmptyStack;
			return 0;
		}

		m_Marker -= DATASIZE8;
		return m_Memory[m_Marker];
	}
	long long Pop64i()
	{
		if(m_Marker < DATASIZE64)
		{
			m_LastError = Koala::Utility::Text::Pop64iOnEmptyStack;
			return 0;
		}

		m_Marker -= DATASIZE64;
		return *((long long*)&m_Memory[m_Marker]);
	}
	double Pop64f()
	{
		if(m_Marker < DATASIZE64)
		{
			m_LastError = Koala::Utility::Text::Pop64fOnEmptyStack;
			return 0;
		}

		m_Marker -= DATASIZE64;
		return *((double*)&m_Memory[m_Marker]);
	}

	char Top8()
	{
		if(m_Marker < DATASIZE8)
		{
			m_LastError = Koala::Utility::Text::Top8OnEmptyStack;
			return 0;
		}

		return m_Memory[m_Marker - DATASIZE8];
	}
	long long Top64i()
	{
		if(m_Marker < DATASIZE64)
		{
			m_LastError = Koala::Utility::Text::Top64iOnEmptyStack;
			return 0;
		}

		return *((long long*)&m_Memory[m_Marker - DATASIZE64]);
	}
	double Top64f()
	{
		if(m_Marker < DATASIZE64)
		{
			m_LastError = Koala::Utility::Text::Top64fOnEmptyStack;
			return 0;
		}

		return *((double*)&m_Memory[m_Marker - DATASIZE64]);
	}

	/**
	*	Get the last error message (if happend)
	*	
	*	@returns the last error message - Text::Empty if no error occured
	*/
	Koala::Utility::Text GetLastError()
	{
		return m_LastError;
	}

private:
	static constexpr size_t DATASIZE8 = sizeof(char);
	static constexpr size_t DATASIZE64 = sizeof(long long);

private:
	char* m_Memory = nullptr;
	size_t m_MemorySize = 0;
	size_t m_Marker = 0;

	Koala::Utility::Text m_LastError = Koala::Utility::Text::Empty;

};

} // namespace Koala::VirtualMachine::Tool

#endif // KOALA__VIRTUAL_MACHINE__TOOL__STACK
