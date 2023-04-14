#ifndef _COORD_CONTAINER_BASE_H_
#define _COORD_CONTAINER_BASE_H_

#include <iostream>
#include <vector>

#include "Point3d.h"
#include "PointSphere.h"

namespace EnvProj
{
    template <typename T>
    class CoordContainerBase
    {
    public:
        virtual Point3d<T> GetClosestPointCartesian(T azim, T evel) = 0;
        virtual Point3d<T> GetClosestPointCartesian(T x, T y, T z) = 0;

        virtual PointSphere<T> GetClosestPointSpherical(T azim, T evel) = 0;
        virtual PointSphere<T> GetClosestPointSpherical(T x, T y, T z) = 0;
    };
}


#endif