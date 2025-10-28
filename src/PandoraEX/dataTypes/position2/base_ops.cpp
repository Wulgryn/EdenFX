#include "../../position2.hpp"

using namespace PandoraEX;

Position2 &Position2::operator=(const Position2 &b2)
{
    x = b2.x;
    y = b2.y;
    return *this;
}

double &Position2::operator[](const int &index)
{
    return index == 0 ? x : y;
}