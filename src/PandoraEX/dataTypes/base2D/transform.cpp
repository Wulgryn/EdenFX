#include "transform.hpp"
#include "typenames.hpp"
//#include "math.hpp"

#include <iostream>
using namespace std;

#include <cmath>
#include <algorithm>

using namespace PandoraEX::Base2D;

template <class Type>
Transform<Type>::Transform(double &val1, double &val2) : val1(val1), val2(val2)
{
    static_assert(is_base_of<Transform<Type>, Type>::value, "T must derive from Base2D::Transform<T>");
}

template <class Type>
Type &Transform<Type>::normalize(const Type &b2)
{
    val1 /= ((Transform)b2).val1;
    val2 /= ((Transform)b2).val2;
    return static_cast<Type&>(*this);
}

template <class Type>
Type &Transform<Type>::normalize(const double &b2)
{
    val1 /= b2;
    val2 /= b2;
    return static_cast<Type&>(*this);
}

template <class Type>
Type &Transform<Type>::normalize()
{
    double len = sqrt(val1 * val1 + val2 * val2);
    val1 /= len;
    val2 /= len;
    return static_cast<Type&>(*this);
}

// template <class T>
// double Transform<T>::angle() const
// {
//     return atan2(val2, val1);
// }

// template <class T>
// T &Transform<T>::setAngle(const T &v2)
// {
//     double len = sqrt(val1 * val1 + val2 * val2);
//     val1 = len * cos(((Transform)v2).angle());
//     val2 = len * sin(((Transform)v2).angle());
//     return static_cast<T&>(*this);
// }

// template <class T>
// T &Transform<T>::setAngle(const double &angle)
// {
//     double len = sqrt(val1 * val1 + val2 * val2);
//     val1 = len * cos(angle);
//     val2 = len * sin(angle);
//     return static_cast<T&>(*this);
// }

// template <class T>
// T &Transform<T>::rotate(const T &v2)
// {
//     double cosA = cos(v2.angle());
//     double sinA = sin(v2.angle());
//     double x = this->x;
//     this->x = x * cosA - this->y * sinA;
//     this->y = x * sinA + this->y * cosA;
//     return static_cast<T&>(*this);
// }

// template <class T>
// T &Transform<T>::rotate(const T &v2, const T &vCenter)
// {
//     double cosA = cos(v2.angle());
//     double sinA = sin(v2.angle());
//     double x = this->x - vCenter.x;
//     this->x = x * cosA - (this->y - vCenter.y) * sinA + vCenter.x;
//     this->y = x * sinA + (this->y - vCenter.y) * cosA + vCenter.y;
//     return static_cast<T&>(*this);
// }

// template <class T>
// T &Transform<T>::rotate(const double &angle)
// {
//     double cosA = cos(angle);
//     double sinA = sin(angle);
//     double x = this->x;
//     this->x = x * cosA - this->y * sinA;
//     this->y = x * sinA + this->y * cosA;
//     return static_cast<T&>(*this);
// }

// template <class T>
// T &Transform<T>::rotate(const double &angle, const T &vCenter)
// {
//     double cosA = cos(angle);
//     double sinA = sin(angle);
//     double x = this->x - vCenter.x;
//     this->x = x * cosA - (this->y - vCenter.y) * sinA + vCenter.x;
//     this->y = x * sinA + (this->y - vCenter.y) * cosA + vCenter.y;
//     return static_cast<T&>(*this);
// }

// template <class T>
// T &Transform<T>::lerp(const T &v2, const double &amount)
// {
//     x = x + (v2.x - x) * amount;
//     y = y + (v2.y - y) * amount;
//     return static_cast<T&>(*this);
// }

// template <class T>
// T &Transform<T>::lerp(const T &v2, const T &v3, const double &amount)
// {
//     x = x + (v2.x - v3.x) * amount;
//     y = y + (v2.y - v3.y) * amount;
//     return static_cast<T&>(*this);
// }

// template <class T>
// T &Transform<T>::reflect(const T &normal)
// {
//     T n = normal.clone();
//     n.normalize();
//     double _dot = this->dot(n);
//     x = x - 2 * _dot * n.x;
//     y = y - 2 * _dot * n.y;
//     return static_cast<T&>(*this);
// }

// template <class T>
// T &Transform<T>::setLength(double length)
// {
//     normalize();
//     x *= length;
//     y *= length;
//     return static_cast<T&>(*this);
// }

template <class Type>
Type &Transform<Type>::zero()
{
    val1 = 0;
    val2 = 0;
    return static_cast<Type&>(*this);
}

template <class Type>
Type &Transform<Type>::one()
{
    val1 = 1;
    val2 = 1;
    return static_cast<Type&>(*this);
}

template <class Type>
Type &Transform<Type>::up(bool forceSet)
{
    val2 += 1;
    if (forceSet)
    {
        val1 = 0;
        val2 = 1;
    }
    return static_cast<Type&>(*this);
}

template <class Type>
Type &Transform<Type>::down(bool forceSet)
{
    val2 -= 1;
    if (forceSet)
    {
        val1 = 0;
        val2 = -1;
    }
    return static_cast<Type&>(*this);
}

template <class Type>
Type &Transform<Type>::left(bool forceSet)
{
    val1 -= 1;
    if (forceSet)
    {
        val1 = -1;
        val2 = 0;
    }
    return static_cast<Type&>(*this);
}

template <class Type>
Type &Transform<Type>::right(bool forceSet)
{
    val1 += 1;
    if (forceSet)
    {
        val1 = 1;
        val2 = 0;
    }
    return static_cast<Type&>(*this);
}

// template <class T>
// T &Transform<T>::moveTowards(const T &v2, double maxDistanceDelta)
// {
//     T v = v2.clone();
//     T dir = v.clone();
//     dir -= static_cast<T&>(*this);
//     double dist = dir.length();
//     if (dist <= maxDistanceDelta || dist == 0)
//     {
//         x = v.x;
//         y = v.y;
//     }
//     else
//     {
//         dir.normalize();
//         x += dir.x * maxDistanceDelta;
//         y += dir.y * maxDistanceDelta;
//     }
//     return static_cast<T&>(*this);
// }

// template <class T>
// T &Transform<T>::moveTowards(const T &v2, const T &v3, double maxDistanceDelta)
// {
//     T v = v2.clone();
//     T dir = v.clone();
//     dir -= v3;
//     double dist = dir.length();
//     if (dist <= maxDistanceDelta || dist == 0)
//     {
//         x = v.x;
//         y = v.y;
//     }
//     else
//     {
//         dir.normalize();
//         x += dir.x * maxDistanceDelta;
//         y += dir.y * maxDistanceDelta;
//     }
//     return static_cast<T&>(*this);
// }

// template <class T>
// T &Transform<T>::smoothDamp(const T &v2, const T &v3, T &currentVelocity, double smoothTime)
// {
//     double deltaTime = 1;
//     double maxSpeed = INFINITY;
//     return smoothDamp(v2, v3, currentVelocity, smoothTime, maxSpeed, deltaTime);
// }

// template <class T>
// T &Transform<T>::smoothDamp(const T &v2, const T &v3, T &currentVelocity, double smoothTime, double maxSpeed, double deltaTime)
// {
//     double deltaTimeClamped = std::min(deltaTime, 0.1);
//     double omega = 2.0 / smoothTime;
//     //double x = omega * deltaTimeClamped;
//     //double exp = 1.0 / (1.0 + x + 0.48 * x * x + 0.235 * x * x * x);
//     T change = v3.clone();
//     change -= v2;
//     //T originalTo = v2.clone();
//     T maxChange = v3.clone();
//     maxChange *= maxSpeed;
//     change.min(maxChange);
//     change.max(-maxChange);
//     T temp = currentVelocity.clone();
//     T temp2 = change.clone();
//     temp2 *= omega;
//     temp2 *= deltaTimeClamped;
//     temp2 += temp;
//     temp2.min(change);
//     change = temp2;
//     T target = v2.clone();
//     change *= deltaTimeClamped;
//     target += change;
//     T temp3 = v3.clone();
//     T temp4 = v2.clone();
//     temp3 -= temp4;
//     T temp5 = target.clone();
//     temp5 -= v2;
//     bool done = temp3.dot(temp5) > 0;
//     if (done)
//     {
//         target = v3;
//         change = target.clone();
//         change -= v2;
//     }
//     currentVelocity = change;
//     x = v2.x;
//     y = v2.y;
//     x = target.x;
//     y = target.y;
//     return static_cast<T&>(*this);
// }