#ifndef _SPHEREPOINT_H_
#define _SPHEREPOINT_H_

#include <cstdint>

template <typename T>
struct SpherePoint
{
    T azimuth, elevation;
    uint32_t pixelValue;
};

using SpherePointf = SpherePoint<float>;

#endif