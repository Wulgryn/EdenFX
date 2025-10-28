#include "../../position2.hpp"

using namespace PandoraEX;

Position2 Position2::operator+(const double& val) const
{
    return Position2(x + val, y + val);
}

Position2 &Position2::operator+=(const double& b2)
{
    x += b2;
    y += b2;
    return *this;
}

Position2 &Position2::operator++()
{
    ++x;
    ++y;
    return *this;
}

Position2 &Position2::operator++(int)
{
    x++;
    y++;
    return *this;
}