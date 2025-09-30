#ifndef DEBUG_LOG_HPP
#define DEBUG_LOG_HPP

#include <string>
#include <fstream>

namespace PandoraDebug
{
    enum class DebugLogClockPrecision
    {
        SECONDS,
        MILLISECONDS,
        MICROSECONDS,
        NANOSECOND
    };

    class DebugLog
    {
        static std::string getLogFileName();
        static std::string getTime();

        static DebugLog __self;
        static std::string __logPath;

        std::ofstream __currentlogStream;
        std::ofstream __latestLogStream;

        void __print(const std::string &message);

        DebugLog();
        ~DebugLog();
    public:
        static DebugLogClockPrecision clockPrecision;

        /// @brief Path to the log file
        /// @param path Path to the log file. Default is "logs/"
        static void setLogPath(std::string path = "logs/");
        

        static void log(const std::string &message, ...);

        static void logError(const std::string &message, ...);
        static void logInfo(const std::string &message, ...);
        static void logWarning(const std::string &message, ...);
        static void logDebug(const std::string &message, ...);
        static void logTrace(const std::string &message, ...);
        static void logFatal(const std::string &message, ...);
        static void logSuccess(const std::string &message, ...);
        static void logFailure(const std::string &message, ...);
        static void logCritical(const std::string &message, ...);
        static void logException(const std::string &message, ...);
    };
    typedef DebugLog DL;
}

#endif // DEBUG_LOG_HPP