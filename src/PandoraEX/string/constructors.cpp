#include "../string.hpp"

using namespace PandoraEX;

String::String() : std::string() {}

String::String(const char *str) : std::string(str) {}

String::String(const std::string &str) : std::string(str) {}

String::String(const String &str) : std::string(str) {}

String::String(const int &value) : std::string(std::to_string(value)) {}