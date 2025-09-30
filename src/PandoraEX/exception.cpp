#include "exception.hpp"

#include "../PandoraDebug/console.hpp"

#include <iostream>
#include <typeinfo>
#include <memory>
#include <cxxabi.h>
#include <stacktrace>

using namespace PandoraEX;

bool Exceptions::logExceptions = true;

Exceptions::Exception::Exception(String message) noexcept
    : std::exception(), _message(message)
{
}

PandoraEX::Exceptions::Exception::Exception(String message, String exception_class, String file, String line) noexcept
    : std::exception(), _message(exception_class + " -> "+ message + "\n where():  " + file + ":" + line + "\n stacktrace:\n" + std::to_string(std::stacktrace::current()))
{
}

PandoraEX::Exceptions::Exception::Exception(String message, String exception_class, String file, String line, String func) noexcept
    : std::exception(), _message(exception_class + " -> " + message + "\n where():  " + file + ":" + line + " << " + func + "\n stacktrace:\n" + std::to_string(std::stacktrace::current()))
{
}

PandoraEX::Exceptions::Exception::Exception(String message, String file, int line) noexcept
    : std::exception(), _message("Exception -> " + message + "\n where():  " + file + ":" + std::to_string(line) + "\n stacktrace:\n" + std::to_string(std::stacktrace::current()))
{
}

PandoraEX::Exceptions::Exception::Exception(String message, String file, int line, String func) noexcept
    : std::exception(), _message("Exception -> " + message + "\n where():  " + file + ":" + std::to_string(line) + " << " + func + "\n stacktrace:\n" + std::to_string(std::stacktrace::current()))
{
}

const char *Exceptions::Exception::what() const noexcept
{
    return _message.c_str();
}

void Exceptions::Exception::print() const noexcept
{
    std::cerr << _message << std::endl;
}

String Exceptions::Exception::message() const noexcept
{
    return _message;
}

String Exceptions::Exception::className() const noexcept
{
    int status = 0;
    std::unique_ptr<char[], void (*)(void *)> res{
        abi::__cxa_demangle(typeid(*this).name(), nullptr, nullptr, &status),
        std::free};
    return (status == 0) ? res.get() : typeid(*this).name();
}

void Exceptions::Exception::throw_()
{
    if(logExceptions) PandoraDebug::DC::logException(_message);
    throw *this;
}

void Exceptions::Exception::log(bool forceLogToFile)
{
    if (logExceptions || forceLogToFile)
    {
        PandoraDebug::DC::logException(_message);
    }
}