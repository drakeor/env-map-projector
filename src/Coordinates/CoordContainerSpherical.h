#ifndef _COORD_CONTAINER_SPHERICAL_H_
#define _COORD_CONTAINER_SPHERICAL_H_

#include "CoordContainerBase.h"

#include <vector>
#include <mutex>

namespace EnvProj
{
    template <typename T>
    class CoordContainerSpherical : public CoordContainerBase<T>
    {
    public:
        CoordContainerSpherical(unsigned int azimVectorSize, unsigned int evelVectorSize);
        bool SetPoint(T azim, T evel, PointSphere<T> point);

        Point3d<T> GetClosestPointCartesian(T azim, T evel) = 0;
        Point3d<T> GetClosestPointCartesian(T x, T y, T z) = 0;

        PointSphere<T> GetClosestPointSpherical(T azim, T evel) = 0;
        PointSphere<T> GetClosestPointSpherical(T x, T y, T z) = 0;

    private:
        unsigned int AzimElevToIndex(T azim, T evel);

        std::vector<PointSphere<T>> points;
        std::mutex mtx;
        unsigned int azimVectorSize;
        unsigned int evelVectorSize;
    };
}


#endif