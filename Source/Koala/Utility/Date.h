#ifndef KOALA__UTILITY__DATE
#define KOALA__UTILITY__DATE

#include <string>

namespace Koala::Utility {

class Date
{
public:
	Date()
	{ }
	Date(size_t day, size_t month, size_t year);

	static const Date Now();

	void SetDay(size_t day);
	void SetMonth(size_t month);
	void SetYear(size_t day);

	size_t GetDay() const
	{
		return m_Day;
	}
	size_t GetMonth() const
	{
		return m_Month;
	}
	size_t GetYear() const
	{
		return m_Year;
	}

	operator const std::string() const;
	const std::string ToString() const;

private:
	size_t m_Day = 1;
	size_t m_Month = 1;
	size_t m_Year = 1970;

};

} // namespace Koala::Utility

#endif // KOALA__UTILITY__DATE
