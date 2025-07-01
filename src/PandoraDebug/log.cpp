#include "log.hpp"

#include "dateTime.hpp"
#include "utils.hpp"

#include <iostream>
#include <stdarg.h>
#include <fstream>
#include <filesystem>
using namespace std;

using namespace PandoraDebug;
using namespace PandoraDebug;
using namespace PandoraDebug::Utils;

string DebugLog::getLogFileName()
{
    string logFileName = "edenFX_";
    logFileName += to_string(DateTime::Year()) + "-";
    logFileName += string(pad(DateTime::Month())) + "-";
    logFileName += string(pad(DateTime::Day())) + "_";
    logFileName += string(pad(DateTime::Hour())) + ".";
    logFileName += string(pad(DateTime::Minute())) + ".";
    logFileName += string(pad(DateTime::Second())) + ".log";

    return logFileName;
}

string DebugLog::getTime()
{

    string time = "[" + pad(DateTime::Month());
    time += "." + pad(DateTime::Day());
    time += "|" + pad(DateTime::Hour());
    time += ":" + pad(DateTime::Minute());
    time += ":" + pad(DateTime::Second());

    switch (clockPrecision)
    {
    case DebugLogClockPrecision::SECONDS:
        return time + "]";
    case DebugLogClockPrecision::MILLISECONDS:
        return time + "." + pad(DateTime::Millisecond(), 3) + "]";
    case DebugLogClockPrecision::MICROSECONDS:
        return time + "." + pad(DateTime::Millisecond(), 3) + "." + pad(DateTime::Microsecond(), 3) + "]";
    case DebugLogClockPrecision::NANOSECOND:
        return time + "." + pad(DateTime::Millisecond(), 3) + "." + pad(DateTime::Microsecond(), 3) + "." + pad(DateTime::Nanosecond(), 3) + "]";
    default:
        return time + "]";
    }
}

DebugLog DebugLog::__self;
DebugLogClockPrecision DebugLog::clockPrecision = DebugLogClockPrecision::MILLISECONDS;
string DebugLog::__logPath = "logs/";

DebugLog::DebugLog()
{
    if(__logPath.empty())
    {
        __logPath = "logs/";
    }


    if(__logPath.back() != '/')
    {
        __logPath += "/";
    }
    namespace fs = std::filesystem;
    if (!fs::exists(__logPath.c_str()))
    {
        fs::create_directory(__logPath.c_str());
    }

    __currentlogStream = ofstream(__logPath + getLogFileName(), ios::app);
    if (!__currentlogStream)
    {
        std::cerr << "Failed to open log file: " << __logPath + getLogFileName() << std::endl;
    }

    __latestLogStream = ofstream(__logPath + "latest.log", ios::trunc);
    if (!__latestLogStream)
    {
        std::cerr << "Failed to open log file: " << __logPath + "latest.log" << std::endl;
    }
}

DebugLog::~DebugLog()
{
    __currentlogStream.close();
    __latestLogStream.close();
}

void DebugLog::__print(const string &message)
{
    __currentlogStream << message;
    __latestLogStream << message;
    __currentlogStream.flush();
    __latestLogStream.flush();
}

void DebugLog::setLogPath(string path)
{
    __logPath = path;
    if(__logPath.empty())
    {
        __logPath = "logs/";
    }

    if(__logPath.back() != '/')
    {
        __logPath += "/";
    }
    namespace fs = std::filesystem;
    if (!fs::exists(__logPath.c_str()))
    {
        fs::create_directory(__logPath.c_str());
    }

    __self.__currentlogStream.close();
    __self.__latestLogStream.close();

    __self.__currentlogStream.open(__logPath + getLogFileName(), ios::app);
    if (!__self.__currentlogStream)
    {
        std::cerr << "Failed to open log file: " << __logPath + getLogFileName() << std::endl;
    }

    __self.__latestLogStream.open(__logPath + "latest.log", ios::trunc);
    if (!__self.__latestLogStream)
    {
        std::cerr << "Failed to open log file: " << __logPath + "latest.log" << std::endl;
    }
}

void DebugLog::log(const string &message, ...)
{
    va_list args;
    va_start(args, message);

    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(nullptr, 0, (getTime() + " " + message + "\n").c_str(), args_copy);
    va_end(args_copy);
    char *formattedMessage = new char[len + 1];
    sprintf(formattedMessage, (getTime() + " " + message + "\n").c_str(), args);

    __self.__print(formattedMessage);

    va_end(args);
}

void DebugLog::logError(const string &message, ...)
{
    va_list args;
    va_start(args, message);

    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(nullptr, 0, (getTime() + " [ERROR] " + message + "\n").c_str(), args_copy);
    va_end(args_copy);
    char *formattedMessage = new char[len + 1];
    sprintf(formattedMessage, (getTime() + " [ERROR] " + message + "\n").c_str(), args);

    __self.__print(formattedMessage);

    va_end(args);
}

void DebugLog::logInfo(const string &message, ...)
{
    va_list args;
    va_start(args, message);

    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(nullptr, 0, (getTime() + " [INFO] " + message + "\n").c_str(), args_copy);
    va_end(args_copy);
    char *formattedMessage = new char[len + 1];
    sprintf(formattedMessage, (getTime() + " [INFO] " + message + "\n").c_str(), args);

    __self.__print(formattedMessage);

    va_end(args);
}

void DebugLog::logWarning(const string &message, ...)
{
    va_list args;
    va_start(args, message);

    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(nullptr, 0, (getTime() + " [WARNING] " + message + "\n").c_str(), args_copy);
    va_end(args_copy);
    char *formattedMessage = new char[len + 1];
    sprintf(formattedMessage, (getTime() + " [WARNING] " + message + "\n").c_str(), args);

    __self.__print(formattedMessage);

    va_end(args);
}

void DebugLog::logDebug(const string &message, ...)
{
    va_list args;
    va_start(args, message);

    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(nullptr, 0, (getTime() + " [DEBUG] " + message + "\n").c_str(), args_copy);
    va_end(args_copy);
    char *formattedMessage = new char[len + 1];
    sprintf(formattedMessage, (getTime() + " [DEBUG] " + message + "\n").c_str(), args);

    __self.__print(formattedMessage);

    va_end(args);
}

void DebugLog::logTrace(const string &message, ...)
{
    va_list args;
    va_start(args, message);

    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(nullptr, 0, (getTime() + " [TRACE] " + message + "\n").c_str(), args_copy);
    va_end(args_copy);
    char *formattedMessage = new char[len + 1];
    sprintf(formattedMessage, (getTime() + " [TRACE] " + message + "\n").c_str(), args);

    __self.__print(formattedMessage);

    va_end(args);
}

void DebugLog::logFatal(const string &message, ...)
{
    va_list args;
    va_start(args, message);

    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(nullptr, 0, (getTime() + " [FATAL] " + message + "\n").c_str(), args_copy);
    va_end(args_copy);
    char *formattedMessage = new char[len + 1];
    sprintf(formattedMessage, (getTime() + " [FATAL] " + message + "\n").c_str(), args);

    __self.__print(formattedMessage);

    va_end(args);
}

void DebugLog::logSuccess(const string &message, ...)
{
    va_list args;
    va_start(args, message);

    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(nullptr, 0, (getTime() + " [SUCCESS] " + message + "\n").c_str(), args_copy);
    va_end(args_copy);
    char *formattedMessage = new char[len + 1];
    sprintf(formattedMessage, (getTime() + " [SUCCESS] " + message + "\n").c_str(), args);

    __self.__print(formattedMessage);

    va_end(args);
}

void DebugLog::logFailure(const string &message, ...)
{
    va_list args;
    va_start(args, message);

    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(nullptr, 0, (getTime() + " [FAILURE] " + message + "\n").c_str(), args_copy);
    va_end(args_copy);
    char *formattedMessage = new char[len + 1];
    sprintf(formattedMessage, (getTime() + " [FAILURE] " + message + "\n").c_str(), args);

    __self.__print(formattedMessage);

    va_end(args);
}

void DebugLog::logCritical(const string &message, ...)
{
    va_list args;
    va_start(args, message);

    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(nullptr, 0, (getTime() + " [CRITICAL] " + message + "\n").c_str(), args_copy);
    va_end(args_copy);
    char *formattedMessage = new char[len + 1];
    sprintf(formattedMessage, (getTime() + " [CRITICAL] " + message + "\n").c_str(), args);

    __self.__print(formattedMessage);

    va_end(args);
}

void DebugLog::logException(const string &message, ...)
{
    va_list args;
    va_start(args, message);

    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(nullptr, 0, (getTime() + " [EXCEPTION] " + message + "\n").c_str(), args_copy);
    va_end(args_copy);
    char *formattedMessage = new char[len + 1];
    sprintf(formattedMessage, (getTime() + " [EXCEPTION] " + message + "\n").c_str(), args);

    __self.__print(formattedMessage);

    va_end(args);
}