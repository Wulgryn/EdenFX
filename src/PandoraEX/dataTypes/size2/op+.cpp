#include "../../size2.hpp"

using namespace PandoraEX;


Size2 Size2::operator+(const double &b2) const
{
    return Size2(width + b2, height + b2);
}


Size2 &Size2::operator+=(const double &b2)
{
    width += b2;
    height += b2;
    return *this;
}


Size2 &Size2::operator++()
{
    ++width;
    ++height;
    return *this;
}

Size2 &Size2::operator++(int)
{
    width++;
    height++;
    return *this;
}