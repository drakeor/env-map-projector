#include "CoordConversions.h"

using namespace std;
using namespace EnvProj;

template<typename T>
PointSphere<T> CoordConversions<T>::CartesianToSpherical(Eigen::Vector3<T> point)
{
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