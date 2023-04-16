#include "CoordConversions.h"

using namespace std;
using namespace EnvProj;

template<typename T>
PointSphere<T> CoordConversions<T>::CartesianToSpherical(Eigen::Vector3<T> point)
{
    // This should map 1:1 to SphericalToCartesian
    PointSphere<T> newPoint;
    newPoint.azimuth = atan2f(point.y(), point.x());
    newPoint.elevation = atan2f(point.z(), sqrtf(point.x() * point.x() + point.y() * point.y()));
    return newPoint;
}

template<typename T>
Eigen::Vector3<T> CoordConversions<T>::SphericalToCartesian(PointSphere<T> point)
{
    // Remember that r=1 since everything is done against the unit sphere.
    Eigen::Vector3<T> newPoint(
        cosf(point.elevation) * cosf(point.azimuth),
        cosf(point.elevation) * sinf(point.azimuth),
        sinf(point.elevation)
    );
    return newPoint;
}

template<typename T> 
Eigen::Vector3i CoordConversions<T>::SideToCoordMap(SkyboxSurf side)
{
    switch(side)
    {
        // Top-Bottom plane is simple xy plane. z = {-1, 1}.
        case TopSurf:
        case BottomSurf:
            return Eigen::Vector3i(0, 1, 2);

        // Left-Right is the xz plane. y = {-1, 1}.
        case LeftSurf:
        case RightSurf:
            return Eigen::Vector3i(0, 2, 1);

        // Front-Back is the yz plane. x = {-1, 1}.
        case FrontSurf:
        case BackSurf:
            return Eigen::Vector3i(2, 0, 1);
    }

    return Eigen::Vector3i(0, 1, 2);
}

// Inverse of function above
template<typename T> 
Eigen::Vector3i CoordConversions<T>::SideToCoordMapInv(SkyboxSurf side)
{
    switch(side)
    {
        // Top/Bottom permutations are involutory, so just 
        // return the same as the forward direction
        case TopSurf:
        case BottomSurf:
            return Eigen::Vector3i(0, 1, 2);

        // Same with left/right.
        case LeftSurf:
        case RightSurf:
            return Eigen::Vector3i(0, 2, 1);

        // Front/Back is the only one NOT involutory,
        // but easy to calculate as it's just the inverse permutation matrix.
        case FrontSurf:
        case BackSurf:
            return Eigen::Vector3i(1, 2, 0);
    }

    return Eigen::Vector3i(0, 1, 2);
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