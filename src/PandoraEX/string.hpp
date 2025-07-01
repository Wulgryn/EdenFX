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

        //String operator+(const unsigned char str);
        //String operator+(const unsigned short str);

/**=======================================================================================================================*
 **                                           END OF REGION OPERATORS
 *========================================================================================================================*/
#pragma endregion OPERATORS
    };
}

#endif // STRING_HPP