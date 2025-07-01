#include "dateTime.hpp"

// nano < micro < milli < sec < min < hour < day < week < month < year

#include <chrono>
#include <ctime>

using namespace PandoraEX;
DateTime::DateTime() {}
DateTime::~DateTime() {}

unsigned long long DateTime::Tick()
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
}

unsigned short DateTime::Nanosecond()
{
    return static_cast<unsigned short>(Tick() % 1000);
}

unsigned short DateTime::Microsecond()
{
    return static_cast<unsigned short>((Tick() / 1000) % 1000);
}

unsigned short DateTime::Millisecond()
{
    return static_cast<unsigned short>((Tick() / 1000000) % 1000);
}

unsigned char DateTime::Second()
{
    std::time_t now = std::time(nullptr);
    std::tm *tm = std::localtime(&now);
    return static_cast<__UINT8_TYPE__>(tm->tm_sec);
}

unsigned char DateTime::Minute()
{
    std::time_t now = std::time(nullptr);
    std::tm *tm = std::localtime(&now);
    return static_cast<unsigned char>(tm->tm_min);
}

unsigned char DateTime::Hour()
{
    std::time_t now = std::time(nullptr);
    std::tm *tm = std::localtime(&now);
    return static_cast<unsigned char>(tm->tm_hour);
}

unsigned char DateTime::Day()
{
    std::time_t now = std::time(nullptr);
    std::tm *tm = std::localtime(&now);
    return static_cast<unsigned char>(tm->tm_mday);
}

unsigned char DateTime::DayOfWeek()
{
    std::time_t now = std::time(nullptr);
    std::tm *tm = std::localtime(&now);
    return static_cast<unsigned char>(tm->tm_wday);
}

unsigned short DateTime::DayOfYear()
{
    std::time_t now = std::time(nullptr);
    std::tm *tm = std::localtime(&now);
    return static_cast<unsigned short>(tm->tm_yday + 1);
}

unsigned char DateTime::Month()
{
    std::time_t now = std::time(nullptr);
    std::tm *tm = std::localtime(&now);
    return static_cast<unsigned char>(tm->tm_mon + 1);
}

unsigned short DateTime::Year()
{
    std::time_t now = std::time(nullptr);
    std::tm *tm = std::localtime(&now);
    return static_cast<unsigned short>(tm->tm_year + 1900);
}
