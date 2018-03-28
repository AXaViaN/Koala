#include <Koala/Utility/Clock.h>
#include <chrono>
#include <ctime>

namespace Koala::Utility {

static auto g_ProgramStart = std::chrono::system_clock::now();

Clock::Clock(size_t hour, size_t minute, size_t second)
{
	SetHour(hour);
	SetMinute(minute);
	SetSecond(second);
}

const Clock Clock::Now()
{
	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now);
	auto tmDate = std::localtime(&now_c);

	return Clock(tmDate->tm_hour, tmDate->tm_min, tmDate->tm_sec);
}
const Clock Clock::ProgramClock()
{
	auto now = std::chrono::system_clock::now();
	auto programDuration = now - g_ProgramStart;

	auto hour = std::chrono::duration_cast<std::chrono::hours>(programDuration);
	auto minute = std::chrono::duration_cast<std::chrono::minutes>(programDuration);
	auto second = std::chrono::duration_cast<std::chrono::seconds>(programDuration);

	Clock clock;
	clock.SetHour(hour.count());
	clock.SetMinute(minute.count() % 60);
	clock.SetSecond(second.count() % 60);

	return clock;
}

void Clock::SetHour(size_t hour)
{
	m_Hour = hour;
}
void Clock::SetMinute(size_t minute)
{
	if(minute > 59)
	{
		minute = 59;
	}

	m_Minute = minute;
}
void Clock::SetSecond(size_t second)
{
	if(second > 59)
	{
		second = 59;
	}

	m_Second = second;
}

Clock::operator const std::string() const
{
	std::string hour = std::to_string(m_Hour);
	std::string minute = std::to_string(m_Minute);
	std::string second = std::to_string(m_Second);

	if(hour.size() < 2)
	{
		hour = "0" + hour;
	}
	if(minute.size() < 2)
	{
		minute = "0" + minute;
	}
	if(second.size() < 2)
	{
		second = "0" + second;
	}

	return hour + ":" + minute + ":" + second;
}
const std::string Clock::ToString() const
{
	return *this;
}

} // namespace Koala::Utility
