#ifndef _COORD_CONTAINER_CARTESIAN_H_
#define _COORD_CONTAINER_CARTESIAN_H_

#include "CoordContainerBase.h"

namespace EnvProj
{
    template <typename T>
    class CoordContainerSkybox : public CoordContainerBase<T>
    {
    public:
        Point3d<T> GetClosestPointCartesian(T azim, T evel) = 0;
        Point3d<T> GetClosestPointCartesian(T x, T y, T z) = 0;

        PointSphere<T> GetClosestPointSpherical(T azim, T evel) = 0;
        PointSphere<T> GetClosestPointSpherical(T x, T y, T z) = 0;
    };
}

#endif