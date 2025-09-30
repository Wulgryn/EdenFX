#include "utils.hpp"

#include <memory>
#include <cxxabi.h>
#include <cstdarg>
using namespace std;
using namespace PandoraDebug;

string Utils::pad(int value, int width)
{
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%0*d", width, value);
    return string(buffer);
}

string Utils::pad(string val, int width)
{
    return pad(std::atoi(val.c_str()), width);
}

std::string Utils::format(const std::string &format, ...)
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
    return std::string(buffer.get());
}