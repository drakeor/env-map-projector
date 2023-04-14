#include "CoordContainerSpherical.h"
#include "CoordConversions.h"

using namespace std;
using namespace EnvProj;

const float pi = 3.14159265358979323846f;


template<typename T>
unsigned int CoordContainerSpherical<T>::AzimElevToIndex(T azim, T evel)
{
    // Convert azim/evel to coordinates
    unsigned int x = (unsigned int)((azim / (T)(2*pi)) * (T)azimVectorSize);
    unsigned int y = (unsigned int)((evel / (T)(pi)) * (T)evelVectorSize);

    // Constrain to range
    if(x < 0)
        x = 0;
    if(x >= azimVectorSize)
        x = azimVectorSize - 1;
    if(y < 0)
        y = 0;
    if(y >= evelVectorSize)
        y = evelVectorSize - 1;

    return (y* azimVectorSize + x);
}

template<typename T>
CoordContainerSpherical<T>::CoordContainerSpherical(unsigned int _azimVectorSize, unsigned int _evelVectorSize)
{
    azimVectorSize = _azimVectorSize;
    evelVectorSize = _evelVectorSize;

    mtx.lock();
    points.reserve(azimVectorSize * evelVectorSize);
    mtx.unlock();
}

template<typename T>
bool CoordContainerSpherical<T>::SetPoint(T azim, T evel, PointSphere<T> point)
{
    mtx.lock();
    auto i = AzimToIndex(azim, evel);
    points[i] = point;
    mtx.unlock();

    return true;
}


template<typename T>
Point3d<T> CoordContainerSpherical<T>::GetClosestPointCartesian(T azim, T evel)
{
    // Grab point
    mtx.lock();
    auto i = AzimToIndex(azim, evel);
    PointSphere<T> point = points[i];
    mtx.unlock();

    // Convert to cartesian
    Point3d<T> newPoint = SphericalToCartesian<T>(point);
    return newPoint;
}

template<typename T>
Point3d<T> CoordContainerSpherical<T>::GetClosestPointCartesian(T x, T y, T z)
{
    // Convert to Spherical first
    Point3d<T> inputPoint;
    inputPoint.x = x;
    inputPoint.y = y;
    inputPoint.z = z;
    PointSphere<T> convPoint = CartesianToSpherical<T>(inputPoint);

    // Grab point
    mtx.lock();
    auto i = AzimToIndex(convPoint.azimuth, convPoint.elevation);
    PointSphere<T> point = points[i];
    mtx.unlock();

    // Convert back to cartesian
    Point3d<T> newPoint = SphericalToCartesian<T>(point);
    return newPoint;
}

template<typename T>
PointSphere<T> CoordContainerSpherical<T>::GetClosestPointSpherical(T azim, T evel)
{
    // Grab point directly
    mtx.lock();
    auto i = AzimToIndex(azim, evel);
    PointSphere<T> point =  points[i];
    mtx.unlock();

    return point;
}

template<typename T>
PointSphere<T> CoordContainerSpherical<T>::GetClosestPointSpherical(T x, T y, T z)
{
    // Convert to Spherical first
    Point3d<T> inputPoint;
    inputPoint.x = x;
    inputPoint.y = y;
    inputPoint.z = z;
    PointSphere<T> convPoint = CartesianToSpherical<T>(inputPoint);

    // Grab point
    mtx.lock();
    auto i = AzimToIndex(convPoint.azimuth, convPoint.elevation);
    PointSphere<T> point = points[i];
    mtx.unlock();

    return point;
}