#ifndef BASE2D_PRIMITIVES_VECTOR2D_HPP
#define BASE2D_PRIMITIVES_VECTOR2D_HPP

#include "../vectorGeometryMath.hpp"
#include "../base2D.hpp"
#include "../utils.hpp"

namespace PandoraEX::Base2D
{
    template <class Type>
    Struct(Vector2D) extends Base2D<Type>, Utils, VectorGeometry2D
    {
        double x = 0, y = 0;
        Vector2D() : Base2D<Type>(x, y), Utils(x, y), VectorGeometry2D(x, y)  {}

        template <class U>
        bool operator==(const U &b2) const
        {
            /* ADD: class test
             *+ *=================================ADD===================================
             *+ * DESCRIPTION: to prevent wrong comparison between different classes
             *+ *=======================================================================
             *+ */
            // if (this* == b2)
            // {
            //     return true;
            // }
            return length() == b2.length() && angle() == b2.angle();
        }
        bool operator==(const double &b2) const
        {
            return length() == b2 && angle() == b2;
        }

        template <class U>
        bool operator!=(const U &b2) const
        {
            return length() != b2.length() || angle() != b2.angle();
        }
        bool operator!=(const double &b2) const
        {
            return length() != b2 || angle() != b2;
        }

        template <class U>
        bool operator>(const U &b2) const
        {
            return length() > b2.length();
        }
        bool operator>(const double &b2) const
        {
            return length() > b2;
        }

        template <class U>
        bool operator<(const U &b2) const
        {
            return length() < b2.length();
        }
        bool operator<(const double &b2) const
        {
            return length() < b2;
        }

        template <class U>
        bool operator>=(const U &b2) const
        {
            return length() >= b2.length();
        }
        bool operator>=(const double &b2) const
        {
            return length() >= b2;
        }

        template <class U>
        bool operator<=(const U &b2) const
        {
            return length() <= b2.length();
        }
        bool operator<=(const double &b2) const
        {
            return length() <= b2;
        }

        bool operator!() const
        {
            return !x && !y;
        }

        template <class U>
        bool operator&&(const Base2D<U> &b2) const
        {
            return x && y && b2.x && b2.y;
        }
        bool operator&&(const double &b2) const
        {
            return x && y && b2;
        }

        template <class U>
        bool operator||(const Base2D<U> &b2) const
        {
            return x || y || b2.x || b2.y;
        }
        bool operator||(const double &b2) const
        {
            return x || y || b2;
        }
    };

} // namespace PandoraEX::Base2D

#endif // BASE2D_PRIMITIVES_VECTOR2D_HPP