#include "../string.hpp"
#include <cstdarg>
#include <memory>
using namespace PandoraEX;

String String::pad(int value, int width)
{
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%0*d", width, value);
    return String(buffer);
}

String String::pad(String val, int width)
{
    return pad(std::atoi(val.c_str()), width);
}

String String::format(const String &format, ...)
{
    va_list args;
    va_start(args, format);
    va_list args_copy;
    va_copy(args_copy, args);
    int length = vsnprintf(nullptr, 0, format.c_str(), args_copy);
    va_end(args_copy);
    std::unique_ptr<char[]> buffer(new char[length + 1]);
    vsnprintf(buffer.get(), length + 1, format.c_str(), args);
    va_end(args);
    return String(buffer.get());
}