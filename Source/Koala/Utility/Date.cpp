#include <Koala/Utility/Date.h>
#include <chrono>
#include <ctime>

namespace Koala::Utility {

Date::Date(size_t day, size_t month, size_t year)
{
	SetDay(day);
	SetMonth(month);
	SetYear(year);
}

const Date Date::Now()
{
	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now);
	auto tmDate = std::localtime(&now_c);

	Date date;
	date.SetDay(tmDate->tm_mday);
	date.SetMonth(tmDate->tm_mon + 1);
	date.SetYear(tmDate->tm_year + 1900);

	return date;
}

void Date::SetDay(size_t day)
{
	if(day < 1)
	{
		day = 1;
	}
	else if(day > 31)
	{
		day = 31;
	}

	m_Day = day;
}
void Date::SetMonth(size_t month)
{
	if(month < 1)
	{
		month = 1;
	}
	else if(month > 12)
	{
		month = 12;
	}

	m_Month = month;
}
void Date::SetYear(size_t year)
{
	m_Year = year;
}

Date::operator const std::string() const
{
	std::string day = std::to_string(m_Day);
	std::string month = std::to_string(m_Month);
	std::string year = std::to_string(m_Year);

	if(day.size() < 2)
	{
		day = "0" + day;
	}
	if(month.size() < 2)
	{
		month = "0" + month;
	}

	return day + "." + month + "." + year;
}
const std::string Date::ToString() const
{
	return *this;
}

} // namespace Koala::Utility
