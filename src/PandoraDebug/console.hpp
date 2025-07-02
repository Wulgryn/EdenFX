#ifndef PANDORADEBUG_CONSOLE_HPP
#define PANDORADEBUG_CONSOLE_HPP

// #include "../PandoraEX/string.hpp"
#include "utils.hpp"
#include <string>
namespace PandoraDebug
{
    enum class ConsoleForegroundColors;
    enum class ConsoleBackgroundColors;
    /// @brief Enumeration for the precision of the debug console clock.
    enum class DebugConsoleClockPrecision
    {
        /// @brief No precision, only the date is displayed.
        SECONDS,
        /// @brief Precision to the second, displaying hours, minutes, and seconds.
        MILLISECONDS,
        /// @brief Precision to the millisecond, displaying hours, minutes, seconds, and milliseconds.
        MICROSECONDS,
        /// @brief Precision to the microsecond, displaying hours, minutes, seconds, milliseconds, and microseconds.
        NANOSECOND
    };

    class DebugConsole
    {
        static std::string getTime();

        static void mf_writeLine(const std::string &message, ConsoleForegroundColors fg_color,
                                 ConsoleBackgroundColors bg_color);

        static void mf_log(const std::string &message);
        static void mf_logError(const std::string &message);
        static void mf_logInfo(const std::string &message);
        static void mf_logWarning(const std::string &message);
        static void mf_logDebug(const std::string &message);
        static void mf_logTrace(const std::string &message);
        static void mf_logFatal(const std::string &message);
        static void mf_logSuccess(const std::string &message);
        static void mf_logFailure(const std::string &message);
        static void mf_logCritical(const std::string &message);
        static void mf_logException(const std::string &message);

        static bool sm_isAttached;
        static bool sm_isVirtualConsole;
        static void *sm_virtualConsoleHandle;
        static unsigned long sm_virtualConsolePID;
        static unsigned char sm_dll[];
        static unsigned int sm_dllSize;
        static std::string sm_dllName;

        static void mf_detach(bool asNewConsole);
        static void mf_attach();

        static unsigned long mf_VCH_Init();
        static void mf_VCH_Write(const char *msg);
        static void mf_VCH_SetTextAttribute(int attribute);

        static int mf_VCH_CloseHandler(unsigned long ctrlType);

    public:
        /// @brief Sets if the debug console should log messages to a file.
        /// @note The default value is true, meaning messages will be logged to a file.
        /// @note If set to false, messages will only be printed to the console.
        static bool logToFile;
        /// @brief Sets the precision of the debug console clock.
        /// @note The default precision is milliseconds.
        static DebugConsoleClockPrecision clockPrecision;
#ifdef EDEN_DEBUG
        /// @brief Logs a message to the console and optionally to a file.
        /// @param message The message to log.
        /// @param args The arguments to format the message with.
        template <typename... Args>
        static void log(const std::string &msg, Args... args)
        {
            mf_log(Utils::format(msg, std::forward<Args>(args)...));
        }

        /// @brief Logs an error message to the console and optionally to a file.
        /// @param message The error message to log.
        /// @param args The arguments to format the message with.
        template <typename... Args>
        static void logError(const std::string &message, Args... args)
        {
            mf_logError(Utils::format("[ERROR]" + message, std::forward<Args>(args)...));
        }

        /// @brief Logs an informational message to the console and optionally to a file.
        /// @param message The informational message to log.
        /// @param args The arguments to format the message with.
        template <typename... Args>
        static void logInfo(const std::string &message, Args... args)
        {
            mf_logInfo(Utils::format("[INFO]" + message, std::forward<Args>(args)...));
        }

        /// @brief Logs a warning message to the console and optionally to a file.
        /// @param message The warning message to log.
        /// @param args The arguments to format the message with.
        template <typename... Args>
        static void logWarning(const std::string &message, Args... args)
        {
            mf_logWarning(Utils::format("[WARNING]" + message, std::forward<Args>(args)...));
        }

        /// @brief Logs a debug message to the console and optionally to a file.
        /// @param message The debug message to log.
        /// @param args The arguments to format the message with.
        template <typename... Args>
        static void logDebug(const std::string &message, Args... args)
        {
            mf_logDebug(Utils::format("[DEBUG]" + message, std::forward<Args>(args)...));
        }

        /// @brief Logs a trace message to the console and optionally to a file.
        /// @param message The trace message to log.
        /// @param args The arguments to format the message with.
        template <typename... Args>
        static void logTrace(const std::string &message, Args... args)
        {
            mf_logTrace(Utils::format("[TRACE]" + message, std::forward<Args>(args)...));
        }

        /// @brief Logs a fatal error message to the console and optionally to a file.
        /// @param message The fatal error message to log.
        /// @param args The arguments to format the message with.
        template <typename... Args>
        static void logFatal(const std::string &message, Args... args)
        {
            mf_logFatal(Utils::format("[FATAL]" + message, std::forward<Args>(args)...));
        }

        /// @brief Logs a success message to the console and optionally to a file.
        /// @param message The success message to log.
        /// @param args The arguments to format the message with.
        template <typename... Args>
        static void logSuccess(const std::string &message, Args... args)
        {
            mf_logSuccess(Utils::format("[SUCCESS]" + message, std::forward<Args>(args)...));
        }

        /// @brief Logs a failure message to the console and optionally to a file.
        /// @param message The failure message to log.
        /// @param args The arguments to format the message with.
        template <typename... Args>
        static void logFailure(const std::string &message, Args... args)
        {
            mf_logFailure(Utils::format("[FAILURE]" + message, std::forward<Args>(args)...));
        }

        /// @brief Logs a critical message to the console and optionally to a file.
        /// @param message The critical message to log.
        /// @param args The arguments to format the message with.
        template <typename... Args>
        static void logCritical(const std::string &message, Args... args)
        {
            mf_logCritical(Utils::format("[CRITICAL]" + message, std::forward<Args>(args)...));
        }

        /// @brief Logs an exception message to the console and optionally to a file.
        /// @param message The exception message to log.
        /// @param args The arguments to format the message with.
        template <typename... Args>
        static void logException(const std::string &message, Args... args)
        {
            mf_logException(Utils::format("[EXCEPTION]" + message, std::forward<Args>(args)...));
        }

        /// @brief Checks if the debug console is currently attached to a console.
        /// @return True if the debug console is attached, false otherwise.
        /// @note This function is platform-specific and may not work on all platforms.
        static bool isAttached()
        {
            return sm_isAttached;
        }

        /// @brief Checks if the debug console is a virtual console.
        /// @return True if the debug console is a virtual console, false otherwise.
        /// @note A virtual console is a console that is not directly associated with a terminal or command prompt, such as a console created by the application itself.
        /// @note This function is platform-specific and may not work on all platforms.
        static bool isVirtualConsole()
        {
            return sm_isVirtualConsole;
        }

        /// @brief Detaches the debug console from the current console.
        /// @param asNewConsole If true, detaches from the original console, but leaves it open and creates a new console. `Default is false.`
        /// @note This function is platform-specific and may not work on all platforms.
        static void detach(bool asNewConsole = false)
        {
            mf_detach(asNewConsole);
        }

        /// @brief Attaches the debug console to the current console.
        /// @note This function is platform-specific and may not work on all platforms.
        static void attach()
        {
            mf_attach();
        }
#else
        /// @warning EDEN_DEBUG is not defined, logging functions will not log anything.
        /// @note This is useful for release builds where logging is not needed or desired.
        template <typename... Args>
        static void log(const std::string &, Args...) {}

        /// @warning EDEN_DEBUG is not defined, logging functions will not log anything.
        /// @note This is useful for release builds where logging is not needed or desired.
        template <typename... Args>
        static void logError(const std::string &, Args...) {}

        /// @warning EDEN_DEBUG is not defined, logging functions will not log anything.
        /// @note This is useful for release builds where logging is not needed or desired.
        template <typename... Args>
        static void logInfo(const std::string &, Args...) {}

        /// @warning EDEN_DEBUG is not defined, logging functions will not log anything.
        /// @note This is useful for release builds where logging is not needed or desired.
        template <typename... Args>
        static void logWarning(const std::string &, Args...) {}

        /// @warning EDEN_DEBUG is not defined, logging functions will not log anything.
        /// @note This is useful for release builds where logging is not needed or desired.
        template <typename... Args>
        static void logDebug(const std::string &, Args...) {}

        /// @warning EDEN_DEBUG is not defined, logging functions will not log anything.
        /// @note This is useful for release builds where logging is not needed or desired.
        template <typename... Args>
        static void logTrace(const std::string &, Args...) {}

        /// @warning EDEN_DEBUG is not defined, logging functions will not log anything.
        /// @note This is useful for release builds where logging is not needed or desired.
        template <typename... Args>
        static void logFatal(const std::string &, Args...) {}

        /// @warning EDEN_DEBUG is not defined, logging functions will not log anything.
        /// @note This is useful for release builds where logging is not needed or desired.
        template <typename... Args>
        static void logSuccess(const std::string &, Args...) {}

        /// @warning EDEN_DEBUG is not defined, logging functions will not log anything.
        /// @note This is useful for release builds where logging is not needed or desired.
        template <typename... Args>
        static void logFailure(const std::string &, Args...) {}

        /// @warning EDEN_DEBUG is not defined, logging functions will not log anything.
        /// @note This is useful for release builds where logging is not needed or desired.
        template <typename... Args>
        static void logCritical(const std::string &, Args...) {}

        /// @warning EDEN_DEBUG is not defined, logging functions will not log anything.
        /// @note This is useful for release builds where logging is not needed or desired.
        template <typename... Args>
        static void logException(const std::string &, Args...) {}

        /// @warning EDEN_DEBUG is not defined.
        /// @return Always returns false, indicating the debug console is not attached.
        /// @note This is useful for release builds where debugging is not needed or desired.
        static bool isAttached() { return false; }

        /// @warning EDEN_DEBUG is not defined.
        /// @return Always returns false, indicating the debug console is not a virtual console.
        /// @note This is useful for release builds where debugging is not needed or desired.
        static bool isVirtualConsole() { return false; }

        /// @warning EDEN_DEBUG is not defined, detach function will not do anything.
        /// @note This is useful for release builds where debugging is not needed or desired.
        static void detach(bool = false) {}

        /// @warning EDEN_DEBUG is not defined, attach function will not do anything.
        /// @note This is useful for release builds where debugging is not needed or desired.
        static void attach() {}
#endif
    };
    typedef DebugConsole DC;
}

#endif // PANDORADEBUG_CONSOLE_HPP