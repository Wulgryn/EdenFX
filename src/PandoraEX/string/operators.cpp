#include "../string.hpp"

using namespace PandoraEX;

String &String::operator=(const char *str)
{
    std::string::operator=(str);
    return *this;
}

String &String::operator=(const String &str)
{
    std::string::operator=(str);
    return *this;
}

// String String::operator+(const unsigned char str)
// {
//     return String(*this + std::to_string(str));
// }

// String String::operator+(const unsigned short str)
// {
//     return String(*this + std::to_string(str));
// }