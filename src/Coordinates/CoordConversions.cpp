#include "CoordConversions.h"

using namespace std;
using namespace EnvProj;

template<typename T>
PointSphere<T> CartesianToSpherical(Point3d<T> point)
{
        PointSphere<T> newPoint;
        newPoint.azimuth = atan2(point.y, point.x);
        newPoint.elevation = atan2(point.z, sqrt(point.x * point.x + point.y * point.y));
        newPoint.pixelValue = point.pixelValue;
        return newPoint;
}

template<typename T>
Point3d<T> SphericalToCartesian(PointSphere<T> point)
{
    // Remember that r=1 since everything is done against the unit sphere.
    Point3d<T> newPoint;
    newPoint.x = cos(point.elevation) * cos(point.azimuth);
    newPoint.y = cos(point.elevation) * sin(point.azimuth);
    newPoint.z = sin(point.elevation);
    newPoint.pixelValue = point.pixelValue;
    return newPoint;
}