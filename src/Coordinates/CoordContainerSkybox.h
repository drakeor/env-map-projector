#ifndef _COORD_CONTAINER_CARTESIAN_H_
#define _COORD_CONTAINER_CARTESIAN_H_

#include "CoordContainerBase.h"

namespace EnvProj
{
    template <typename T>
    class CoordContainerSkybox : public CoordContainerBase<T>
    {
    public:
        CoordContainerSkybox(uint32_t _sideVectorLength);
        bool SetPoint(T x, T y, T z, uint32_t data);
/*
        Point3d<T> GetClosestPointCartesian(T azim, T evel) = 0;
        Point3d<T> GetClosestPointCartesian(T x, T y, T z) = 0;
        PointSphere<T> GetClosestPointSpherical(T x, T y, T z) = 0;
        PointSphere<T> GetClosestPointSpherical(T azim, T evel) = 0;
*/
        uint32_t GetClosestPixel(T azim, T evel) = 0;

    private:
        uint32_t CartesianToIndex(T x, T y, T z);

        std::vector<uint32_t> points;
        std::mutex mtx;
        uint32_t sideVectorLength;
    };
}

#endif