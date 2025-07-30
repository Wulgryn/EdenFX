#include "../../size2.hpp"

using namespace PandoraEX;

Size2 &Size2::operator=(const Size2 &b2)
{
    width = b2.width;
    height = b2.height;
    return *this;
}

double &Size2::operator[](const int &index)
{
    return index == 0 ? width : height;
}
