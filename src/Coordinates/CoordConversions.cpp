#include "CoordConversions.h"

using namespace std;
using namespace EnvProj;

template<typename T>
PointSphere<T> CoordConversions<T>::CartesianToSpherical(Point3d<T> point)
{
    // This should map 1:1 to SphericalToCartesian
    PointSphere<T> newPoint;
    newPoint.azimuth = atan2f(point.y, point.x);
    newPoint.elevation = atan2f(point.z, sqrtf(point.x * point.x + point.y * point.y));
    return newPoint;
}

template<typename T>
Point3d<T> CoordConversions<T>::SphericalToCartesian(PointSphere<T> point)
{
    // Remember that r=1 since everything is done against the unit sphere.
    Point3d<T> newPoint({
        cosf(point.elevation) * cosf(point.azimuth),
        cosf(point.elevation) * sinf(point.azimuth),
        sinf(point.elevation)
    });
    return newPoint;
}

template<typename T> 
Point3d<int32_t> CoordConversions<T>::SideToCoordMap(SkyboxSurf side)
{
    switch(side)
    {
        // Top-Bottom plane is simple xy plane. z = {-1, 1}.
        case TopSurf:
        case BottomSurf:
            return Point3d<int32_t>({0, 1, 2});

        // Left-Right is the xz plane. y = {-1, 1}.
        case LeftSurf:
        case RightSurf:
            return Point3d<int32_t>({0, 2, 1});

        // Front-Back is the yz plane. x = {-1, 1}.
        case FrontSurf:
        case BackSurf:
            return Point3d<int32_t>({2, 0, 1});
    }

    return Point3d<int32_t>({0, 1, 2});
}

// Inverse of function above
template<typename T> 
Point3d<int32_t> CoordConversions<T>::SideToCoordMapInv(SkyboxSurf side)
{
    switch(side)
    {
        // Top/Bottom permutations are involutory, so just 
        // return the same as the forward direction
        case TopSurf:
        case BottomSurf:
            return Point3d<int32_t>({0, 1, 2});

        // Same with left/right.
        case LeftSurf:
        case RightSurf:
            return Point3d<int32_t>({0, 2, 1});

        // Front/Back is the only one NOT involutory,
        // but easy to calculate as it's just the inverse permutation matrix.
        case FrontSurf:
        case BackSurf:
            return Point3d<int32_t>({1, 2, 0});
    }

    return Point3d<int32_t>({0, 1, 2});
}

template<typename T> 
T CoordConversions<T>::SideToConstVal(SkyboxSurf side)
{
    switch(side)
    {
        // These are on positive axis
        case BottomSurf:
        case BackSurf:
        case RightSurf:
            return 1.0f;

        // These are on negative axis
        case TopSurf:
        case LeftSurf:
        case FrontSurf:
            return -1.0f;
    }

    return 1.0f;
}

namespace EnvProj {
    template class CoordConversions<float>;
    template class CoordConversions<double>;
}