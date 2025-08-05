#ifndef PANDORAEX_SIZE2_HPP
#define PANDORAEX_SIZE2_HPP

#include "PandoraEX/object.hpp"
#include "dataTypes/base2D/math.hpp"
#include "dataTypes/base2D/transform.hpp"
#include "dataTypes/base2D/primitives/size2D.hpp"

namespace PandoraEX
{
/* [ ] FIX: fix comments
 *& *================================FIX====================================
 *& * DESCRIPTION: fix the style of the comments, and the names from x,y to val1, val2
 *& * HINT: highlight val1 and val2 with ``
 *& *=======================================================================
 *& */

/* ADD: descriptions
 *+ *=================================ADD===================================
 *+ * DESCRIPTION: add descriptions
 *+ *=======================================================================
 *+ */

    Struct(Size2) pextends Base2D::Size2D<Size2>, Base2D::Math<Size2>, Base2D::Transform<Size2>
    {
        /// @brief Default constructor.
        Size2();

        /// @brief Constructor with width and height.
        /// @param width_height The width and height to set.
        /// @note This will set both width and height to the same value.
        Size2(double width_height);

        /// @brief Constructor with width and height.
        /// @param width The width to set.
        /// @param height The height to set.
        Size2(double width, double height);
    

        using Base2D::operator=;
        
        /// @brief Copy constructor.
        /// @param other The object to copy from.
        Size2(const Size2 &other) = default;

        /// @brief Assignment operator. Sets width and height to b2.width and b2.height.
        /// @param b2 The object to assign.
        /// @return Size2 The object.
        Size2 &operator=(const Size2 &b2);
        
        /// @brief Index operator.
        /// @param index The index to get. 0 for val1, 1 for val2.
        /// @return double The value at the index.
        /// @note 0 for val1, otherwise val2.
        double &operator[](const int &index);

/**=======================================================================================================================*
**                                                  REGION SIZE2 OP+
*========================================================================================================================*/
#pragma region SIZE2 OP+

        /// @brief Addition operator.
        /// @tparam U The type of the other operand.
        /// @param op The other operand.
        /// @return Size2 The result of the addition.
        template <class U>
        Size2 operator+(const Base2D<U> &op) const
        {
            return Size2(width + op.getVal1(), height + op.getVal2());
        }

        /// @brief Addition operator.
        /// @param val The value to add.
        /// @return Size2 The result of the addition.
        Size2 operator+(const double &val) const;

        /// @brief Addition assignment operator.
        /// @tparam U The type of the other operand.
        /// @param b2 The other operand.
        /// @return Size2& The result of the addition.
        template <class U>
        Size2 &operator+=(const Base2D<U> &b2)
        {
            width += b2.getVal1();
            height += b2.getVal2();
            return *this;
        }

        /// @brief Addition assignment operator.
        /// @param b2 The value to add.
        /// @return Size2& The result of the addition.
        Size2 &operator+=(const double &b2);

        /// @brief Prefix increment operator.
        /// @return Size2& The result of the increment.
        Size2 &operator++();

        /// @brief Postfix increment operator.
        /// @param unused Unused parameter.
        /// @return Size2& The result of the increment.
        Size2 &operator++(int);

/**=======================================================================================================================*
 **                                           END OF REGION SIZE2 OP+
 *========================================================================================================================*/
#pragma endregion SIZE2 OP +

/**=======================================================================================================================*
 **                                                  REGION SIZE2 OP -
 *========================================================================================================================*/
#pragma region SIZE2 OP - -

        /// @brief Subtraction operator.
        /// @tparam U The type of the other operand.
        /// @param b2 The other operand.
        /// @return Size2 The result of the subtraction.
        template <class U>
        Size2 operator-(const Base2D<U> &b2) const
        {
            return Size2(width - b2.getVal1(), height - b2.getVal2());
        }
        /// @brief Subtraction operator.
        /// @param b2 The value to subtract.
        /// @return Size2 The result of the subtraction.
        Size2 operator-(const double &b2) const;
        /// @brief Negation operator.
        /// @return Size2 The result of the negation.
        Size2 operator-() const;

        /// @brief Subtraction assignment operator.
        /// @tparam U The type of the other operand.
        /// @param b2 The other operand.
        /// @return Size2& The result of the subtraction.
        template <class U>
        Size2 &operator-=(const Base2D<U> &b2)
        {
            width -= b2.getVal1();
            height -= b2.getVal2();
            return *this;
        }

        /// @brief Subtraction assignment operator.
        /// @param b2 The value to subtract.
        /// @return Size2& The result of the subtraction.
        Size2 &operator-=(const double &b2);

        /// @brief Prefix decrement operator.
        /// @return Size2& The result of the decrement.
        Size2 &operator--();

        /// @brief Postfix decrement operator.
        /// @param unused Unused parameter.
        /// @return Size2& The result of the decrement.
        Size2 &operator--(int);

/**=======================================================================================================================*
 **                                           END OF REGION SIZE2 OP-
 *========================================================================================================================*/
#pragma endregion SIZE2 OP - -

/**=======================================================================================================================*
 **                                                  REGION SIZE2 OP *
 *========================================================================================================================*/
#pragma region SIZE2 OP*

        /// @brief Multiplication operator.
        /// @tparam U The type of the other operand.
        /// @param b2 The other operand.
        /// @return Size2 The result of the multiplication.
        template <class U>
        Size2 operator*(const Base2D<U> &b2) const
        {
            return Size2(width * b2.getVal1(), height * b2.getVal2());
        }
        /// @brief Multiplication operator.
        /// @param b2 The value to multiply.
        /// @return Size2 The result of the multiplication.
        Size2 operator*(const double &b2) const;

        /// @brief Multiplication assignment operator.
        /// @tparam U The type of the other operand.
        /// @param b2 The other operand.
        /// @return Size2& The result of the multiplication.
        template <class U>
        Size2 &operator*=(const Base2D<U> &b2)
        {
            width *= b2.getVal1();
            height *= b2.getVal2();
            return *this;
        }
        /// @brief Multiplication assignment operator.
        /// @param b2 The value to multiply.
        /// @return Size2& The result of the multiplication.
        Size2 &operator*=(const double &b2);

/**=======================================================================================================================*
 **                                           END OF REGION SIZE2 OP*
 *========================================================================================================================*/
#pragma endregion SIZE2 OP *

/**=======================================================================================================================*
 **                                                  REGION SIZE2 OP/
 *========================================================================================================================*/
#pragma region SIZE2 OP/

        /// @brief Division operator.
        /// @tparam U The type of the other operand.
        /// @param b2 The other operand.
        /// @return Size2 The result of the division.
        template <class U>
        Size2 operator/(const Base2D<U> &b2) const
        {
            return Size2(width / b2.getVal1(), height / b2.getVal2());
        }

        /// @brief Division operator.
        /// @param b2 The value to divide by.
        /// @return Size2 The result of the division.
        Size2 operator/(const double &b2) const;

        /// @brief Division assignment operator.
        /// @tparam U The type of the other operand.
        /// @param b2 The other operand.
        /// @return Size2& The result of the division.
        template <class U>
        Size2 &operator/=(const Base2D<U> &b2)
        {
            width /= b2.getVal1();
            height /= b2.getVal2();
            return *this;
        }

        /// @brief Division assignment operator.
        /// @param b2 The value to divide by.
        /// @return Size2& The result of the division.
        Size2 &operator/=(const double &b2);

/**=======================================================================================================================*
 **                                           END OF REGION SIZE2 OP/
 *========================================================================================================================*/
#pragma endregion SIZE2 OP /
    };
}

#endif // PANDORAEX_SIZE2_HPP