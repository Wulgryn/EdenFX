#ifndef STRING_HPP
#define STRING_HPP

#include <string>

namespace PandoraEX
{
    class String : public std::string
    {
    public:
/**=======================================================================================================================*
 **                                                  REGION CONSTRUCTORS
 *========================================================================================================================*/
#pragma region CONSTRUCTORS

        String();
        String(const char *str);
        String(const std::string &str);
        String(const String &str);

        String(const int &value);

/**=======================================================================================================================*
 **                                           END OF REGION CONSTRUCTORS
 *========================================================================================================================*/
#pragma endregion CONSTRUCTORS

/**=======================================================================================================================*
 **                                                  REGION OPERATORS
 *========================================================================================================================*/
#pragma region OPERATORS

        String &operator=(const char *str);
        String &operator=(const String &str);

        // String operator+(const unsigned char str);
        // String operator+(const unsigned short str);

/**=======================================================================================================================*
 **                                           END OF REGION OPERATORS
 *========================================================================================================================*/
#pragma endregion OPERATORS

        /// @brief Pads a number with zeros to the left.
        /// @param value The number to pad.
        /// @param width The width of the number.
        /// @return The padded number.
        /// @note value = 7, width = 2 -> "07"
        String pad(int value, int width = 2);

        /// @brief Pads a string with zeros to the left.
        /// @param val The string to pad.
        /// @param width The width of the string.
        /// @return The padded string.
        /// @note `value = "7", width = 2 -> "07"`
        /// @note Value must be a "number".
        String pad(String val, int width = 2);

        String format(const String &format, ...);
    };
}

#endif // STRING_HPP