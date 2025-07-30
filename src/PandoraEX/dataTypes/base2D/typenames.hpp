#ifndef BASE2D_TYPENAMES_HPP
#define BASE2D_TYPENAMES_HPP
#pragma once
#include "base2D.hpp"
#include "math.hpp"
#include "transform.hpp"

#include "primitives/size2D.hpp"
#include "primitives/vector2D.hpp"
#include "primitives/point2D.hpp"

#include "../dataTypes2D.hpp"

// template struct PandoraEX::Base2D::Base2D<PandoraEX::Vector2>;
// template struct PandoraEX::Base2D::Base2D<PandoraEX::RVector2>;

template struct PandoraEX::Base2D::Base2D<PandoraEX::Size2>;
// template struct PandoraEX::Base2D::Base2D<PandoraEX::RSize2>;

// template struct PandoraEX::Base2D::Base2D<PandoraEX::Point2>;
// template struct PandoraEX::Base2D::Base2D<PandoraEX::RPoint2>;


// template struct PandoraEX::Base2D::Math<PandoraEX::Vector2>;
template struct PandoraEX::Base2D::Math<PandoraEX::Size2>;
// template struct PandoraEX::Base2D::Math<PandoraEX::Point2>;


// template struct PandoraEX::Base2D::Transform<PandoraEX::Vector2>;
template struct PandoraEX::Base2D::Transform<PandoraEX::Size2>;
// template struct PandoraEX::Base2D::Transform<PandoraEX::Point2>;


// template struct PandoraEX::Base2D::Vector2D<PandoraEX::Vector2>;
// template struct PandoraEX::Base2D::Vector2D<PandoraEX::RVector2>;

template struct PandoraEX::Base2D::Size2D<PandoraEX::Size2>;
// template struct PandoraEX::Base2D::Size2D<PandoraEX::RSize2>;


// template struct PandoraEX::Base2D::Point2D<PandoraEX::Point2>;
// template struct PandoraEX::Base2D::Point2D<PandoraEX::RPoint2>;

#endif // BASE2D_TYPENAMES_HPP