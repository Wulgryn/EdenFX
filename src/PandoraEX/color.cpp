#include "color.hpp"
#include "exception.hpp"
using namespace PandoraEX;

Color::Color(double r, double g, double b, double a)
{
    if(r < 0 || r > 255) ThrowExceptionF(Exceptions::InvalidArgumentException, "Red value must be between 0 and 255 OR 0.0 and 1.0");
    if(r > 1) this->r = r / 255.0;
    else this->r = r;

    if(g < 0 || g > 256) ThrowExceptionF(Exceptions::InvalidArgumentException, "Green value must be between 0 and 255 OR 0.0 and 1.0");
    if(g > 1) this->g = g / 255.0;
    else this->g = g;

    if(b < 0 || b > 256) ThrowExceptionF(Exceptions::InvalidArgumentException, "Blue value must be between 0 and 255 OR 0.0 and 1.0");
    if(b > 1) this->b = b / 255.0;
    else this->b = b;

    if(a < 0 || a > 256) ThrowExceptionF(Exceptions::InvalidArgumentException, "Alpha value must be between 0 and 255 OR 0.0 and 1.0");
    if(a > 1) this->a = a / 255.0;
    else this->a = a;
}