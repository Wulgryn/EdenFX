#ifndef BASE2D_TRANSFORM_HPP
#define BASE2D_TRANSFORM_HPP

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

/* ADD: transroms
 *+ *=================================ADD===================================
 *+ * DESCRIPTION: as separated: [pointMath,VectorMath] rotate,rotateAroundPoint,lerp,reflect,moveTowards,smoothDamp,setLength
 *+ *=======================================================================
 *+ */

namespace PandoraEX::Base2D
{
    template <class Type>
    struct Transform
    {
        Transform(double &val1, double &val2);

        Type &normalize(const Type &b2);
        Type &normalize(const double &b2);
        Type &normalize();

        // double angle() const;

        // T &setAngle(const T &v2);
        // T &setAngle(const double &angle);

        // T &rotate(const T &v2);
        // T &rotate(const T &v2, const T &vCenter);
        // T &rotate(const double &angle);
        // T &rotate(const double &angle, const T &vCenter);

        // T &lerp(const T &v2, const double &amount);
        // T &lerp(const T &v2, const T &v3, const double &amount);

        // T &reflect(const T &normal);

        // T &setLength(double length);

        Type &zero();
        Type &one();
        Type &up(bool forceSet = false);
        Type &down(bool forceSet = false);
        Type &left(bool forceSet = false);
        Type &right(bool forceSet = false);

        // T &moveTowards(const T &v2, double maxDistanceDelta);
        // T &moveTowards(const T &v2, const T &v3, double maxDistanceDelta);

        // T &smoothDamp(const T &v2, const T &v3, T &currentVelocity, double smoothTime);
        // T &smoothDamp(const T &v2, const T &v3, T &currentVelocity, double smoothTime, double maxSpeed, double deltaTime);

        private:
            double &val1, &val2;
    };
    
}

#endif // BASE2D_TRANSFORM_HPP