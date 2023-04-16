#ifndef _COORD_CONVERSIONS_H_
#define _COORD_CONVERSIONS_H_

#include "SkyboxSurf.h"
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
            static Eigen::Vector3i SideToCoordMap(SkyboxSurf side);
            static Eigen::Vector3i SideToCoordMapInv(SkyboxSurf side);
            static T SideToConstVal(SkyboxSurf side);
    };

    template class CoordConversions<float>;
    template class CoordConversions<double>;
}

#endif