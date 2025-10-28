#include "../../position2.hpp"

using namespace PandoraEX;

Position2 Position2::operator*(const double &b2) const
{
    return Position2(x * b2, y * b2);
}

Position2 &Position2::operator*=(const double &b2)
{
    x *= b2;
    y *= b2;
    return *this;
}
