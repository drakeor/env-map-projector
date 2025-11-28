#ifndef _COORD_CONTAINER_HEMISPHERICAL_H_
#define _COORD_CONTAINER_HEMISPHERICAL_H_

#include "CoordContainerBase.h"
#include "HemisphereSurf.h"

#include <vector>
#include <mutex>

//#define DEBUG_PRINT_COORD_CONTAINER_HEMISPHERICAL

namespace EnvProj
{
    template <typename T>
    class CoordContainerHemiSpherical : public CoordContainerBase<T>
    {
    public:
        CoordContainerHemiSpherical(uint32_t _vectorSize);
        bool SetPointDirect(HemisphereSurf surf, uint32_t tex_x, uint32_t tex_y, uint32_t data);
        
        uint32_t GetClosestPixel(T azim, T elev);
        uint32_t GetClosestPixel(T x, T y, T z);

    private:
        uint32_t GetFinalIndex(HemisphereSurf surf, uint32_t tex_x, uint32_t tex_y);

        std::vector<uint32_t> points;
        std::mutex mtx;
        uint32_t vectorSize;
    };

    extern template class CoordContainerHemiSpherical<float>;
    extern template class CoordContainerHemiSpherical<double>;
}

#endif