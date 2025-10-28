#ifndef PANDORAEX_POSITION2_HPP
#define PANDORAEX_POSITION2_HPP

#include "dataTypes/base2D/primitives/Point2D.hpp"
#include "dataTypes/base2D/math.hpp"
#include "dataTypes/base2D/transform.hpp"

namespace PandoraEX
{
    struct Position2 : public Base2D::Point2D<Position2>, Base2D::Math<Position2>, Base2D::Transform<Position2>
    {
        Position2();
        Position2(double xy);
        Position2(double x, double y);

        using Base2D::operator=;
        Position2 &operator=(const Position2 &b2);

        double &operator[](const int &index);

/**=======================================================================================================================*
**                                                  REGION SIZE2 OP+
*========================================================================================================================*/
#pragma region SIZE2 OP+

        template <class U>
        Position2 operator+(const Base2D<U> &op) const
        {
            return Position2(x + op.getVal1(), y + op.getVal2());
        }
        Position2 operator+(const double &val) const;

        template <class U>
        Position2 &operator+=(const Base2D<U> &b2)
        {
            x += b2.getVal1();
            y += b2.getVal2();
            return *this;
        }
        Position2 &operator+=(const double &b2);

        Position2 &operator++();
        Position2 &operator++(int);

/**=======================================================================================================================*
 **                                           END OF REGION SIZE2 OP+
 *========================================================================================================================*/
#pragma endregion SIZE2 OP +

/**=======================================================================================================================*
 **                                                  REGION SIZE2 OP -
 *========================================================================================================================*/
#pragma region SIZE2 OP - -

        template <class U>
        Position2 operator-(const Base2D<U> &b2) const
        {
            return Position2(x - b2.getVal1(), y - b2.getVal2());
        }
        Position2 operator-(const double &b2) const;
        Position2 operator-() const;

        template <class U>
        Position2 &operator-=(const Base2D<U> &b2)
        {
            x -= b2.getVal1();
            y -= b2.getVal2();
            return *this;
        }
        Position2 &operator-=(const double &b2);

        Position2 &operator--();
        Position2 &operator--(int);

/**=======================================================================================================================*
 **                                           END OF REGION SIZE2 OP-
 *========================================================================================================================*/
#pragma endregion SIZE2 OP - -

/**=======================================================================================================================*
 **                                                  REGION SIZE2 OP *
 *========================================================================================================================*/
#pragma region SIZE2 OP*

        template <class U>
        Position2 operator*(const Base2D<U> &b2) const
        {
            return Position2(x * b2.getVal1(), y * b2.getVal2());
        }
        Position2 operator*(const double &b2) const;

        template <class U>
        Position2 &operator*=(const Base2D<U> &b2)
        {
            x *= b2.getVal1();
            y *= b2.getVal2();
            return *this;
        }
        Position2 &operator*=(const double &b2);

/**=======================================================================================================================*
 **                                           END OF REGION SIZE2 OP*
 *========================================================================================================================*/
#pragma endregion SIZE2 OP *

/**=======================================================================================================================*
 **                                                  REGION SIZE2 OP/
 *========================================================================================================================*/
#pragma region SIZE2 OP/

        template <class U>
        Position2 operator/(const Base2D<U> &b2) const
        {
            return Size2(x / b2.getVal1(), y / b2.getVal2());
        }
        Position2 operator/(const double &b2) const;

        template <class U>
        Position2 &operator/=(const Base2D<U> &b2)
        {
            x /= b2.getVal1();
            y /= b2.getVal2();
            return *this;
        }
        Position2 &operator/=(const double &b2);

/**=======================================================================================================================*
 **                                           END OF REGION SIZE2 OP/
 *========================================================================================================================*/
#pragma endregion SIZE2 OP /
    };
}

#endif // PANDORAEX_POSITION2_HPP