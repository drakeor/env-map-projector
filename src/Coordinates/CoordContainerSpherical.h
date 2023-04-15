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
        CoordContainerSpherical(uint32_t azimVectorSize, uint32_t evelVectorSize);
        bool SetPoint(T azim, T evel, uint32_t data);
        
        uint32_t GetClosestPixel(T azim, T evel);
        uint32_t GetClosestPixel(T x, T y, T z);

    private:
        uint32_t AzimElevToIndex(T azim, T evel);

        std::vector<uint32_t> points;
        std::mutex mtx;
        uint32_t azimVectorSize;
        uint32_t evelVectorSize;
    };
}


#endif