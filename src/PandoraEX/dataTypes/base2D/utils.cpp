#include "utils.hpp"
#include "typenames.hpp"

#include <iostream>
using namespace std;

using namespace PandoraEX;

Base2D::Utils::Utils(double &val1, double &val2) : _val1(val1), _val2(val2)
{
}

void Base2D::Utils::print() const
{
    std::cout << "x: " << _val1 << ", y: " << _val2 << std::endl;
}

void Base2D::Utils::print(const String &msg) const
{
    /* ADD: reference to values
     *+ *=================================ADD===================================
     *+ * DESCRIPTION: example print("this is my X: {val1}, and this is my Y: {val2}"), or {0},{1}... as in C#
     *+ *=======================================================================
     *+ */
    std::cout << msg << " x: " << _val1 << ", y: " << _val2 << std::endl;
}

String Base2D::Utils::toString() const
{
    return "x: " + std::to_string(_val1) + ", y: " + std::to_string(_val2);
}