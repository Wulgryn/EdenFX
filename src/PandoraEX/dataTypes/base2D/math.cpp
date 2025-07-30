#include "math.hpp"
#include "typenames.hpp"

#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdint>
#include <algorithm>

using namespace PandoraEX::Base2D;

template <class T>
Math<T>::Math(double &val1, double &val2) : val1(val1), val2(val2)
{
    static_assert(std::is_base_of<Math<T>, T>::value, "T must derive from Base2D::Math<T>");
}

template <class T>
T &Math<T>::min(const T &b2)
{
    val1 = std::min(val1, ((Math)b2).val1);
    val2 = std::min(val2, ((Math)b2).val2);
    return static_cast<T &>(*this);
}

template <class T>
T &Math<T>::min(const double &b2)
{
    val1 = std::min(val1, b2);
    val2 = std::min(val2, b2);
    return static_cast<T &>(*this);
}

template <class T>
T &Math<T>::max(const T &b2)
{
    val1 = std::max(val1, ((Math)b2).val1);
    val2 = std::max(val2, ((Math)b2).val2);
    return static_cast<T &>(*this);
}

template <class T>
T &Math<T>::max(const double &b2)
{
    val1 = std::max(val1, b2);
    val2 = std::max(val2, b2);
    return static_cast<T &>(*this);
}

template <class T>
T &Math<T>::abs()
{
    val1 = std::abs(val1);
    val2 = std::abs(val2);
    return static_cast<T &>(*this);
}

template <class T>
T &Math<T>::ceil()
{
    val1 = std::ceil(val1);
    val2 = std::ceil(val2);
    return static_cast<T &>(*this);
}

template <class T>
T &Math<T>::floor()
{
    val1 = std::floor(val1);
    val2 = std::floor(val2);
    return static_cast<T &>(*this);
}

template <class T>
T &Math<T>::round()
{
    val1 = std::round(val1);
    val2 = std::round(val2);
    return static_cast<T &>(*this);
}

template <class T>
T &Math<T>::invert()
{
    val1 = -val1;
    val2 = -val2;
    return static_cast<T &>(*this);
}

template <class T>
T& Math<T>::reciprocal()
{
    val1 = 1 / val1;
    val2 = 1 / val2;
    return static_cast<T&>(*this);
}

template <class T>
T &Math<T>::negate()
{
    uint64_t bits;
    std::memcpy(&bits, &val1, sizeof(double));
    bits = ~bits;
    std::memcpy(&val1, &bits, sizeof(double));
    
    std::memcpy(&bits, &val2, sizeof(double));
    bits = ~bits;
    std::memcpy(&val2, &bits, sizeof(double));

    return static_cast<T &>(*this);
}

template <class Type>
uint64_t Math<Type>::bits(int index) const
{
    uint64_t bits;
    std::memcpy(&bits, index == 0 ? &val1 : &val2, sizeof(double));
    return bits;
}

template <class T>
T &Math<T>::clamp(const T &min, const T &max)
{
    val1 = std::clamp(val1, ((Math)min).val1, ((Math)max).val1);
    val2 = std::clamp(val2, ((Math)min).val2, ((Math)max).val2);
    return static_cast<T &>(*this);
}

template <class T>
T &Math<T>::clamp(const double &min, const double &max)
{
    val1 = std::clamp(val1, min, max);
    val2 = std::clamp(val2, min, max);
    return static_cast<T &>(*this);
}