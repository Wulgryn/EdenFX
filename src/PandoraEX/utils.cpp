#include "utils.hpp"

#include <memory>
#include <cxxabi.h>

using namespace PandoraEX;

String Utils::demangle(const String name)
{
    int status = 0;
    std::unique_ptr<char[], void (*)(void *)> res{
        abi::__cxa_demangle(name.c_str(), nullptr, nullptr, &status),
        std::free};
    return (status == 0) ? res.get() : name;
}

String Utils::pad(int value, int width)
{
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%0*d", width, value);
    return String(buffer);
}

String Utils::pad(String val, int width)
{
    return pad(std::atoi(val.c_str()), width);
}