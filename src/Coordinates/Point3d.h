#ifndef _POINT3D_H_
#define _POINT3D_H_

#include <cstdint>

namespace EnvProj
{
    template <typename T>
    struct Point3d
    {
        T x, y, z;
    };
}

#endif