#ifndef BASE2D_BASE_HPP
#define BASE2D_BASE_HPP

#include <type_traits>
#include <typeinfo>
#include "PandoraEX/object.hpp"
#include "../../utils.hpp"
#include "../../exception.hpp"

/* [ ] FIX: fix comments
 *& *================================FIX====================================
 *& * DESCRIPTION: fix the style of the comments, and the names from x,y to val1, val2
 *& * HINT: highlight val1 and val2 with ``
 *& *=======================================================================
 *& */

namespace PandoraEX::Base2D
{
    /// @brief Base2D class
    /// @tparam T The derived class, must be derived from Base2D<T>
    template <class Type>
    Struct(Base2D)
    {
        /// @brief Constructor.
        /// @param data The data to set val1 and val2 to.
        Base2D(double &val1, double &val2);

        /// @brief Clones the object.
        /// @return T The cloned object.
        Type clone() const;

        /// @brief Sets the val1 and val2 values.
        /// @param val1 The value to set val1 to.
        /// @param val2 The value to set val2 to.
        /// @return T The object.
        Type &set(double &val1, double &val2);

        template <class U> U to()
        {
            if (!std::is_base_of<Base2D<U>, U>::value) ThrowException(Exceptions::InvalidArgumentException, "Invalid type conversion from " + PandoraEX::Utils::demangle(typeid(*this).name()) + " to " + PandoraEX::Utils::demangle(typeid(U).name()));
            return U(val1, val2);
        }

        /// @brief Assignment operator. Sets val1 to b2.val1 and val2 to b2.val2.
        /// @param b2 The object to assign.
        /// @return T The object.
        template <class U>
        Type &operator=(const Base2D<U> &b2){
            val1 = b2.val1;
            val2 = b2.val2;
            return *this;
        }

        /// @brief Assignment operator. Sets val1 and val2 to b2.
        /// @param b2 The value to assign.
        /// @return T The object.
        Type &operator=(const double &b2);

        double getVal1() const;
        double getVal2() const;

        template <class U>
        operator U() const
        {
            if (!std::is_base_of<Base2D<U>, U>::value) ThrowException(Exceptions::InvalidArgumentException, "Invalid type conversion from " + PandoraEX::Utils::demangle(typeid(*this).name()) + " to " + PandoraEX::Utils::demangle(typeid(U).name()));
            return U(val1, val2);
        }

/* REVIEW: Boost.PFR
 *$ *===============================REVIEW==================================
 *$ * WHY: Optimization
 *$ * DESCRIPTION: More usecases. With boost.pfr, we can use the pfr::get function to get the values of the struct. Or how many values are in the struct.
 *$ * https://github.com/boostorg/pfr/tree/master?tab=readme-ov-file
 *$ *=======================================================================
 *$ */

        // template <PandoraEX::Utils::HasTwoDoubles U>
        // operator U() const
        // {
        //     return U{val1, val2};
        // }

    private:
        double &val1, &val2;
    };
}

#endif // BASE2D_BASE_HPP