#include "base2D.hpp"
#include "typenames.hpp"

#include <type_traits>

using namespace PandoraEX::Base2D;

template <class Type>
Base2D<Type>::Base2D(double &val1, double &val2) : val1(val1), val2(val2)
{
    /* INFO: check out 'concept'
     ** *=======================================================================
     ** * DESCRIPTION: check out keyword concept in C++20.
     ** * It is a way to specify constraints on template parameters.
     ** * It allows you to define a set of requirements that a type must satisfy
     ** * in order to be used with a template.
     ** * Example:
     ** * template <typename T>
     ** *     concept Transformable = std::is_base_of_v<Transform<T>, T>;
     ** * This concept checks if T is derived from Transform<T>.
     ** * You can then use this concept in your template definitions:
     ** * template <Transformable T>
     ** *     void process(T &obj) {
     ** *         // Function implementation
     ** *     }
     ** * This way, the compiler will enforce that T must derive from Transform<T>
     ** *=======================================================================
     ** */
    static_assert(std::is_base_of<Base2D<Type>, Type>::value, "T must derive from Base2D<T>");
}

template <class Type>
Type Base2D<Type>::clone() const
{
    return Type(val1, val2);
}

template <class Type>
Type &Base2D<Type>::set(double &x, double &y)
{
    this->val1 = x;
    this->val2 = y;
    return static_cast<Type &>(*this);
}

template <class Type>
Type &Base2D<Type>::operator=(const double &b2)
{
    val1 = b2;
    val2 = b2;
    return static_cast<Type &>(*this);
}

template <class Type>
double Base2D<Type>::getVal1() const
{
    return val1;
}

template <class Type>
double Base2D<Type>::getVal2() const
{
    return val2;
}