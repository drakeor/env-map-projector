#ifndef _COORD_CONTAINER_BASE_H_
#define _COORD_CONTAINER_BASE_H_

#include <iostream>
#include <vector>

#include "PointSphere.h"

namespace EnvProj
{
    template <typename T>
    class CoordContainerBase
    {
    public:
        virtual uint32_t GetClosestPixel(T azim, T evel) = 0;
        virtual uint32_t GetClosestPixel(T x, T y, T z) = 0;
   };
}


#endif