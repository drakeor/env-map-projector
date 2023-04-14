#ifndef _COORD_CONVERSIONS_H_
#define _COORD_CONVERSIONS_H_

#include "PointSphere.h"
#include "Point3d.h"

namespace EnvProj
{
    template<typename T>
    PointSphere<T> CartesianToSpherical(Point3d<T> point);

    template<typename T>
    Point3d<T> SphericalToCartesian(PointSphere<T> point);
}

#endif