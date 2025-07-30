#ifndef BASE2D_MATH_HPP
#define BASE2D_MATH_HPP

/* [ ] FIX: fix comments
 *& *================================FIX====================================
 *& * DESCRIPTION: fix the style of the comments, and the names from x,y to val1, val2
 *& * HINT: highlight val1 and val2 with ``
 *& *=======================================================================
 *& */

#include <cstdint>

namespace PandoraEX::Base2D
{
    template <class Type>
    struct Math
    {
        Math(double &val1, double &val2);

        /// @brief The minimum of two values.
        /// @param b2 The second vector.
        /// @return T The object.
        /// @note `val1 = min(val1, b2.val1)`
        /// @note `val2 = min(y, b2.val2)`
        Type &min(const Type &b2);

        /// @brief The minimum of val1, val2 and a value.
        /// @param b2 The value to compare.
        /// @return T The object.
        /// @note val1 = min(val1, b2)
        /// @note val2 = min(val2, b2)
        Type &min(const double &b2);

        /// @brief The maximum of two values.
        /// @param b2 The second vector.
        /// @return T The object.
        /// @note val1 = max(val1, b2.val2)
        /// @note val2 = max(val2, b2.val2)
        Type &max(const Type &b2);

        /// @brief The maximum of val1, val2 and a value.
        /// @param b2 The value to compare.
        /// @return T The object.
        /// @note val1 = max(val1, b2)
        /// @note val2 = max(val2, b2)
        Type &max(const double &b2);

        /// @brief The absolute of val1 and val2.
        /// @return T The object.
        Type &abs();

        /// @brief The ceiling of val1 and val2.
        /// @return T The object.
        Type &ceil();

        /// @brief The floor of `val1` and val2.
        /// @return T The object.
        Type &floor();

        /// @brief The round of x and y.
        /// @return T The object.
        Type &round();

        /// @brief The inverse of x and y.
        /// @return T The object.
        Type &invert();

        /// @brief The reciprocal of val1 and val2.
        /// @return T The object.
        Type &reciprocal();

        /// @brief The negate of val1 and val2 with bitwise.
        /// @return T The object.
        /// @warning This is a bitwise operation with double, return value might be NaN;
        Type &negate();

        /// @brief The bits of val1 and val2.
        /// @param index The selected value: 0 for val1, 1 for val2.
        /// @return `utin64_t` The bits of the selected value.
        uint64_t bits(int index) const;

        /// @brief Clamps x and y between min and max.
        /// @param min The minimum value.
        /// @param max The maximum value.
        /// @return T The object.
        Type &clamp(const Type &min, const Type &max);

        /// @brief Clamps x and y between min and max.
        /// @param min The minimum value.
        /// @param max The maximum value.
        /// @return T The object. 
        Type &clamp(const double &min, const double &max);

    private:
        double &val1, &val2;
    };
}

#endif // BASE2D_MATH_HPP