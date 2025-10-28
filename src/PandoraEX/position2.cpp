#include "position2.hpp"

using namespace PandoraEX;

Position2::Position2() : Math<Position2>(x, y), Transform<Position2>(x, y) {}

Position2::Position2(double xy) : Position2() { x = y = xy; }

Position2::Position2(double x, double y) : Position2()
{
    this->x = x;
    this->y = y;
}