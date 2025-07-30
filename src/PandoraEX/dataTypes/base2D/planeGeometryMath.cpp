#include "planeGeometryMath.hpp"
#include "typenames.hpp"


using namespace PandoraEX::Base2D;

PlaneGeometry2D::PlaneGeometry2D(double &val1, double &val2) : val1(val1), val2(val2)
{
    
}

double PlaneGeometry2D::area() const
{
    return val1 * val2;
}

double PlaneGeometry2D::perimeter() const
{
    return 2 * (val1 + val2);
}