#ifndef _POINT3D_H_
#define _POINT3D_H_

template <typename T>
struct Point3d
{
    T x, y, z;
    unsigned int pixelValue;
};

using Point3df = Point3d<float>;

#endif