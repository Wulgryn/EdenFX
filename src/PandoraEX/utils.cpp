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