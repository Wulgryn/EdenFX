#ifndef BASE2D_PRIMITIVES_SIZE2D_HPP
#define BASE2D_PRIMITIVES_SIZE2D_HPP

#include "../planeGeometryMath.hpp"
#include "../utils.hpp"
#include "../base2D.hpp"

namespace PandoraEX::Base2D
{
    template <class Type>
    struct Size2D : Base2D<Type>, PlaneGeometry2D, Utils
    {
        double width = 0, height = 0;
        Size2D() : Base2D<Type>(width, height), PlaneGeometry2D(width, height), Utils(width, height) {}

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
            return area() == b2.area() && perimeter() == b2.perimeter();
        }
        bool operator==(const double &b2) const
        {
            return area() == b2 && perimeter() == b2;
        }

        template <class U>
        bool operator!=(const U &b2) const
        {
            return area() != b2.area() || perimeter() != b2.perimeter();
        }
        bool operator!=(const double &b2) const
        {
            return area() != b2 || perimeter() != b2;
        }

        template <class U>
        bool operator>(const U &b2) const
        {
            return area() > b2.area() && perimeter() > b2.perimeter();
        }
        bool operator>(const double &b2) const
        {
            return area() > b2 && perimeter() > b2;
        }

        template <class U>
        bool operator<(const U &b2) const
        {
            return area() < b2.area() && perimeter() < b2.perimeter();
        }
        bool operator<(const double &b2) const
        {
            return area() < b2 && perimeter() < b2;
        }

        template <class U>
        bool operator>=(const U &b2) const
        {
            return area() >= b2.area() && perimeter() >= b2.perimeter();
        }
        bool operator>=(const double &b2) const
        {
            return area() >= b2 && perimeter() >= b2;
        }

        template <class U>
        bool operator<=(const U &b2) const
        {
            return area() <= b2.area() && perimeter() <= b2.perimeter();
        }
        bool operator<=(const double &b2) const
        {
            return area() <= b2 && perimeter() <= b2;
        }

        bool operator!() const
        {
            return !width && !height;
        }

        template <class U>
        bool operator&&(const Base2D<U> &b2) const
        {
            return width && height && b2.width && b2.height;
        }
        bool operator&&(const double &b2) const
        {
            return width && height && b2;
        }

        template <class U>
        bool operator||(const Base2D<U> &b2) const
        {
            return width || height || b2.width || b2.height;
        }
        bool operator||(const double &b2) const
        {
            return width || height || b2;
        }

/**=======================================================================================================================*
 **                                                  REGION BITWISE OPERATORS
 *========================================================================================================================*/
#pragma region BITWISE OPERATORS
        /* ADD: BITWISE OPERATORS
         *+ *=================================ADD===================================
         *+ * DESCRIPTION: uncomment the bitwise operators from the hpp
         *+ *=======================================================================
         *+ */
        // T operator&(const T &b2);
        // T operator&(const double &b2);

        // T operator|(const T &b2);
        // T operator|(const double &b2);

        // T operator^(const T &b2);
        // T operator^(const double &b2);

        // T operator~() const;

        // T &operator&=(const T &b2);
        // T &operator&=(const double &b2);

        // T &operator|=(const T &b2);
        // T &operator|=(const double &b2);

        // T &operator^=(const T &b2);
        // T &operator^=(const double &b2);

        // T &operator<<=(const T &b2);
        // T &operator<<=(const double &b2);

        // T &operator>>=(const T &b2);
        // T &operator>>=(const double &b2);

        // T operator<<(const T &b2);
        // T operator<<(const double &b2);

        // T operator>>(const T &b2);
        // T operator>>(const double &b2);

/**=======================================================================================================================*
 **                                           END OF REGION BITWISE OPERATORS
 *========================================================================================================================*/
#pragma endregion BITWISE OPERATORS
    };
}

#endif // BASE2D_PRIMITIVES_SIZE2D_HPP