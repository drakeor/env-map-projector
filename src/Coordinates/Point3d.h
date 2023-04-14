#ifndef _POINT3D_H_
#define _POINT3D_H_

namespace EnvProj
{
    template <typename T>
    struct Point3d
    {
        T x, y, z;
        unsigned int pixelValue;
    };
}

#endif