#ifndef _COORD_CONVERSIONS_H_
#define _COORD_CONVERSIONS_H_

#include "PointSphere.h"
#include "../../lib/eigen/Eigen/Dense"

namespace EnvProj
{
    template<typename T>
    class CoordConversions
    {
        public:
            static PointSphere<T> CartesianToSpherical(Eigen::Vector3<T> point);
            static Eigen::Vector3<T> SphericalToCartesian(PointSphere<T> point);
    };

    template class CoordConversions<float>;
    template class CoordConversions<double>;
}

#endif