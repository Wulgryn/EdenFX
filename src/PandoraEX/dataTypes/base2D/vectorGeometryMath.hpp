#ifndef BASE2D_VECTOR_GEOMETRY_MATH_HPP
#define BASE2D_VECTOR_GEOMETRY_MATH_HPP

template <class Type>
struct Vector2D;

#include <cmath>

namespace PandoraEX::Base2D
{
    struct VectorGeometry2D
    {
        VectorGeometry2D(double &val1, double &val2);

        double length() const;
        double magnitude() const;
        double angle() const;
        double direction() const;

        template <class Type>
        double distance(const Vector2D<Type> &vec) const
        {
            return sqrt(pow(val1 - vec.x, 2) + pow(val2 - vec.y, 2));
        }

        template <class Type>
        double dot(const Vector2D<Type> &vec) const
        {
            return val1 * vec.x + val2 * vec.y;
        }

        template <class Type>
        double cross(const Vector2D<Type> &vec) const
        {
            return val1 * vec.y - val2 * vec.x;
        }

        template <class Type>
        double projection(const Vector2D<Type> &vec) const
        {
            return dot(vec) / magnitude();
        }
        template <class Type>
        double rejection(const Vector2D<Type> &vec) const
        {
            return cross(vec) / magnitude();
        }

        template <class Type>
        double vectorProjection(const Vector2D<Type> &vec) const
        {
            return projection(vec) * magnitude();
        }
        template <class Type>
        double vectorRejection(const Vector2D<Type> &vec) const
        {
            return rejection(vec) * magnitude();
        }

        double unit() const;

    private:
        double &val1, &val2;
    };
}

#endif // BASE2D_VECTOR_GEOMETRY_MATH_HPP