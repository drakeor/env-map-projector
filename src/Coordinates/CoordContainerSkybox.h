#ifndef _COORD_CONTAINER_CARTESIAN_H_
#define _COORD_CONTAINER_CARTESIAN_H_

#include "CoordContainerBase.h"

namespace EnvProj
{
    template <typename T>
    class CoordContainerSkybox : public CoordContainerBase<T>
    {
    public:
        CoordContainerSkybox(unsigned int _sideVectorLength);
        bool SetPoint(T x, T y, T z, Point3d<T> point);

        Point3d<T> GetClosestPointCartesian(T azim, T evel) = 0;
        Point3d<T> GetClosestPointCartesian(T x, T y, T z) = 0;

        PointSphere<T> GetClosestPointSpherical(T azim, T evel) = 0;
        PointSphere<T> GetClosestPointSpherical(T x, T y, T z) = 0;

    private:
        unsigned int CartesianToIndex(T x, T y, T z);

        std::vector<PointSphere<T>> points;
        std::mutex mtx;
        unsigned int sideVectorLength;
    };
}

#endif