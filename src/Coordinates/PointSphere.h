#ifndef _POINTSPHERE_H_
#define _POINTSPHERE_H_

#include <cstdint>

namespace EnvProj
{
    template <typename T>
    struct PointSphere
    {
        T azimuth, elevation;
    };
}

#endif