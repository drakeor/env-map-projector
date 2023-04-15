#ifndef _COORD_CONVERSIONS_H_
#define _COORD_CONVERSIONS_H_

#include "PointSphere.h"
#include "../../lib/eigen/Eigen/Dense"

namespace EnvProj
{
    template<typename T>
    PointSphere<T> CartesianToSpherical(Eigen::Vector3<T> point);

    template<typename T>
    Eigen::Vector3<T> SphericalToCartesian(PointSphere<T> point);
}

#endif