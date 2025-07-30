#ifndef PANDORAEX_EXCEPTION_HPP
#define PANDORAEX_EXCEPTION_HPP

#include <exception>
#include "string.hpp"
#include "utils.hpp"
namespace PandoraEX::Exceptions
{
    extern bool logExceptions;
    class Exception : public std::exception
    {
    protected:
        String _message;

    public:
        Exception(String message = "An exception has occurred.") noexcept;
        Exception(String message, String exception_class, String file, String line) noexcept;
        Exception(String message, String exception_class, String file, String line, String func) noexcept;
        Exception(String message, String file, int line) noexcept;
        Exception(String message, String file, int line, String func) noexcept;
        const char *what() const noexcept override;

        /// @brief Prints the exception message to the console.
        void print() const noexcept;

        /// @brief Gets the exception message.
        /// @return The exception message.
        String message() const noexcept;

        /// @brief Gets the exception class name.
        /// @return The exception class name.
        String className() const noexcept;

        /// @brief Throws the exception.
        /// @throws Exception This exception.
        void throw_();

        void log(bool forceLogToFile = false);

        ~Exception() noexcept = default;
    };

/// @brief Creates a new exception object with the given message.
/// @param exceptionClass The class of the exception to create.
/// @param __format The format string for the message.
/// @param ... The arguments to format the message with.
/// @return A new exception object with the given message.
#define Exception_(exceptionClass, __format, ...) PandoraEX::Exceptions::Exception(String::format(__format, ##__VA_ARGS__), PandoraEX::Utils::demangle(typeid(exceptionClass).name()), String(__FILE__), std::to_string(__LINE__))

/// @brief Creates a new exception object with the given message and function name.
/// @param exceptionClass The class of the exception to create.
/// @param __format The format string for the message.
/// @param ... The arguments to format the message with.
/// @return A new exception object with the given message and function name.
#define ExceptionF_(exceptionClass, __format, ...) PandoraEX::Exceptions::Exception(String::format(__format, ##__VA_ARGS__), PandoraEX::Utils::demangle(typeid(exceptionClass).name()), String(__FILE__), std::to_string(__LINE__), __func__)

/// @brief Creates a new exception object with the given message and throws it.
/// @param exceptionClass The class of the exception to create.
/// @param __format The format string for the message.
/// @param ... The arguments to format the message with.
#define ThrowException(exceptionClass, format, ...) {PandoraEX::Exceptions::Exception ex = Exception_(exceptionClass, format, ##__VA_ARGS__); ex.log(true); throw ex;}

/// @brief Creates a new exception object with the given message and function name, and throws it.
/// @param exceptionClass The class of the exception to create.
/// @param format The format string for the message.
/// @param ... The arguments to format the message with.
#define ThrowExceptionF(exceptionClass, format, ...) {PandoraEX::Exceptions::Exception ex = ExceptionF_(exceptionClass, format, ##__VA_ARGS__); ex.log(true); throw ex;}

    class InvalidArgumentException : public Exception
    {
    public:
        InvalidArgumentException(String message = "An invalid argument was passed.") noexcept : Exception(message) {}
        InvalidArgumentException(String message, String file, String line) noexcept : Exception(message, "InvalidArgumentException", file, line) {}
        InvalidArgumentException(String message, String file, String line, String func) noexcept : Exception(message, file, line, func) {}
    };

    class SignatureException : public Exception
    {
    public:
        SignatureException(String message = "Function signature mismatch.") noexcept : Exception(message) {}
        SignatureException(String message, String file, String line) noexcept : Exception(message, "SignatureException", file, line) {}
        SignatureException(String message, String file, String line, String func) noexcept : Exception(message, "SignatureException", file, line, func) {}
        template <typename... Args>
        SignatureException(String signature, String file, String line, Args &&...args) noexcept : Exception("Function signature mismatch -> " + signature, "SignatureException", file, line)
        {
            if (sizeof...(args) > 0)
            {
                this->_message += "\nGiven argument types: ";
                ((this->_message += Utils::demangle(typeid(args).name()) + " "), ...);
            }
        }
    };

    class ArgumentCountException : public Exception
    {
    public:
        ArgumentCountException(String message = "Argument count mismatch.") noexcept : Exception(message) {}
        ArgumentCountException(String message, String file, String line) noexcept : Exception(message, "ArgumentCountException", file, line) {}
        ArgumentCountException(String message, String file, String line, String func) noexcept : Exception(message, "ArgumentCountException", file, line, func) {}
        ArgumentCountException(int exceptedCount, int givenCount, String file, String line) noexcept : Exception("Argument count mismatch. Expected " + String(exceptedCount + "") + " arguments, but got " + String(givenCount + "") + ".", "ArgumentCountException", file, line) {}
        ArgumentCountException(int exceptedCount, int givenCount, String file, String line, String func) noexcept : Exception("Argument count mismatch. Expected " + String(exceptedCount + "") + " arguments, but got " + String(givenCount + "") + ".", "ArgumentCountException", file, line, func) {}
    };

    class NullReferenceException : public Exception
    {
    public:
        NullReferenceException(String message = "Null reference exception.") noexcept : Exception(message) {}
        NullReferenceException(String message, String file, String line) noexcept : Exception(message, "NullReferenceException", file, line) {}
        NullReferenceException(String message, String file, String line, String func) noexcept : Exception(message, "NullReferenceException", file, line, func) {}
    };

    class NotFoundException : public Exception
    {
    public:
        NotFoundException(String message = "Item not found.") noexcept : Exception(message) {}
        NotFoundException(String message, String file, String line) noexcept : Exception(message, "NotFoundException", file, line) {}
        NotFoundException(String message, String file, String line, String func) noexcept : Exception(message, "NotFoundException", file, line, func) {}
    };

    class IndexOutOfBoundsException : public Exception
    {
    public:
        IndexOutOfBoundsException(String message = "Index out of bounds.") noexcept : Exception(message) {}
        IndexOutOfBoundsException(String message, String file, String line) noexcept : Exception(message, "IndexOutOfBoundsException", file, line) {}
        IndexOutOfBoundsException(String message, String file, String line, String func) noexcept : Exception(message, "IndexOutOfBoundsException", file, line, func) {}
    };

}

#endif // PANDORAEX_EXCEPTION_HPP