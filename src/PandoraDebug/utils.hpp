#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

namespace PandoraDebug::Utils
{
    /// @brief Pads a number with zeros to the left.
    /// @param value The number to pad.
    /// @param width The width of the number.
    /// @return The padded number.
    /// @note value = 7, width = 2 -> "07"
    std::string pad(int value, int width = 2);

    /// @brief Pads a string with zeros to the left.
    /// @param val The string to pad.
    /// @param width The width of the string.
    /// @return The padded string.
    /// @note `value = "7", width = 2 -> "07"`
    /// @note Value must be a "number".
    std::string pad(std::string val, int width = 2);

    std::string format(const std::string &format, ...);
}

#endif // UTILS_HPP