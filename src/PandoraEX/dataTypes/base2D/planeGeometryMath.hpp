#ifndef BASE2D_PLANE_GEOMETRY_MATH_HPP
#define BASE2D_PLANE_GEOMETRY_MATH_HPP

#include "PandoraEX/object.hpp"
/* [ ] FIX: fix comments
 *& *================================FIX====================================
 *& * DESCRIPTION: fix the style of the comments, and the names from x,y to val1, val2
 *& * HINT: highlight val1 and val2 with ``
 *& *=======================================================================
 *& */

namespace PandoraEX::Base2D
{
    /// @brief PlaneGeometry2D struct
    Struct(PlaneGeometry2D)
    {
        /// @brief Default constructor.
        /// @param val1 The value to set val1;
        /// @param val2 The value to set val2;
        PlaneGeometry2D(double &val1, double &val2);

        /// @brief The area of the object.
        /// @return the area.
        double area() const;

        /// @brief The perimeter of the object.
        /// @return the perimeter.
        double perimeter() const;

        private:
            double &val1, &val2;
    };
}

#endif // BASE2D_PLANE_GEOMETRY_MATH_HPP