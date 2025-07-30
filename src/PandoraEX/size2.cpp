#include "size2.hpp"

using namespace PandoraEX;
using namespace PandoraEX::Base2D;

Size2::Size2() : Math<Size2>(width, height), Transform<Size2>(width, height) {}

Size2::Size2(double width_height) : Size2() { width = height = width_height; }

Size2::Size2(double width, double height) : Size2()
{
    this->width = width;
    this->height = height;
}