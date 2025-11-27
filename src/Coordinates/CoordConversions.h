#ifndef _COORD_CONVERSIONS_H_
#define _COORD_CONVERSIONS_H_

#include <cstdint>

#include "SkyboxSurf.h"
#include "PointSphere.h"
#include "Point3d.h"
#include <Eigen/Dense>

namespace EnvProj
{
    template<typename T>
    class CoordConversions
    {
        public:
            static PointSphere<T> CartesianToSpherical(Point3d<T> point);
            static Point3d<T> SphericalToCartesian(PointSphere<T> point);
            static Point3d<int32_t> SideToCoordMap(SkyboxSurf side);
            static Point3d<int32_t> SideToCoordMapInv(SkyboxSurf side);
            static T SideToConstVal(SkyboxSurf side);
    };

    template class CoordConversions<float>;
    template class CoordConversions<double>;
}

#endif