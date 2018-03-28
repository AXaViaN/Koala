#ifndef KOALA__UTILITY__CLOCK
#define KOALA__UTILITY__CLOCK

#include <string>

namespace Koala::Utility {

class Clock
{
public:
	Clock()
	{ }
	Clock(size_t hour, size_t minute, size_t second);

	static const Clock Now();
	static const Clock ProgramClock();

	void SetHour(size_t hour);
	void SetMinute(size_t minute);
	void SetSecond(size_t second);

	size_t GetHour() const
	{
		return m_Hour;
	}
	size_t GetMinute() const
	{
		return m_Minute;
	}
	size_t GetSecond() const
	{
		return m_Second;
	}

	operator const std::string() const;
	const std::string ToString() const;

private:
	size_t m_Hour = 0;
	size_t m_Minute = 0;
	size_t m_Second = 0;

};

} // namespace Koala::Utility

#endif // KOALA__UTILITY__CLOCK
