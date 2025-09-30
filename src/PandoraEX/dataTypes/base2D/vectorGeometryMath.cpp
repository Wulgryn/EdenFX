#include "vectorGeometryMath.hpp"
#include "typenames.hpp"

using namespace PandoraEX::Base2D;

VectorGeometry2D::VectorGeometry2D(double &val1, double &val2) : val1(val1), val2(val2)
{
}

double VectorGeometry2D::length() const
{
    return sqrt(pow(val1, 2) + pow(val2, 2));
}

double VectorGeometry2D::magnitude() const
{
    return sqrt(pow(val1, 2) + pow(val2, 2));
}

double VectorGeometry2D::angle() const
{
    return atan2(val2, val1);
}

double VectorGeometry2D::direction() const
{
    return atan2(val2, val1);
}

double VectorGeometry2D::unit() const
{
    return 1 / magnitude();
}
