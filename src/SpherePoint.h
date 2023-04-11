#ifndef _SPHEREPOINT_H_
#define _SPHEREPOINT_H_

template <typename T>
struct SpherePoint
{
    T azimuth, elevation;
    unsigned int pixelValue;
};

using SpherePointf = SpherePoint<float>;

#endif