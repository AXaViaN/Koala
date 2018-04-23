#ifndef KOALA__UTILITY__ID_GENERATOR
#define KOALA__UTILITY__ID_GENERATOR

#include <string>

namespace Koala::Utility {

class IDGenerator
{
public:
	void SetNextID(size_t nextID)
	{
		m_NextID = nextID;
	}
	size_t GetNextID()
	{
		return m_NextID++;
	}

private:
	size_t m_NextID = 1;

};

} // namespace Koala::Utility

#endif // KOALA__UTILITY__ID_GENERATOR
