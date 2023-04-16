#ifndef _COORD_CONTAINER_CARTESIAN_H_
#define _COORD_CONTAINER_CARTESIAN_H_

#include "CoordContainerBase.h"
#include <memory>
#include <mutex>

#define DEBUG_PRINT_COORD_CONTAINER_SKYBOX

namespace EnvProj
{
    template <typename T>
    class CoordContainerSkybox : public CoordContainerBase<T>
    {
    public:
        CoordContainerSkybox(uint32_t _sideVectorLength);
        bool SetPoint(T x, T y, T z, uint32_t data);
        
        uint32_t GetClosestPixel(T azim, T evel);
        uint32_t GetClosestPixel(T x, T y, T z);

    private:
        uint32_t CartesianToIndex(T x, T y, T z);

        std::vector<uint32_t> points;
        std::mutex mtx;
        uint32_t sideVectorLength;
    };

    template class CoordContainerSkybox<float>;
    template class CoordContainerSkybox<double>;
}

#endif