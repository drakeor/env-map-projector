#include "CoordContainerSkybox.h"
#include "CoordConversions.h"

#include "../../lib/eigen/Eigen/Dense"

using namespace std;
using namespace EnvProj;

using Eigen::Vector3;

template <typename T>
CoordContainerSkybox<T>::CoordContainerSkybox(uint32_t _sideVectorLength)
{
    mtx.lock();
    sideVectorLength = _sideVectorLength;
    points.reserve(_sideVectorLength * _sideVectorLength * 6);
    mtx.unlock();
}

template <typename T>
unsigned int CoordContainerSkybox<T>::CartesianToIndex(T x, T y, T z)
{
    // Define some epsilon value for floating point comparisons
    T epsilon = 10e-9f;

    // Relate the boundary to one of the sides
    unsigned int sideIndex = 0;
    
    // Grab from one of the sides based on being on the edge of the skybox.
    T u = 0;
    T v = 0;
    if(abs(x - 1) < epsilon)
    {
        sideIndex = 0;
        u = y;
        v = z;
    } else if(abs(x + 1) < epsilon)
    {
        sideIndex = 1;
        u = y;
        v = z;
    } else if(abs(y - 1) < epsilon)
    {
        sideIndex = 2;
        u = x;
        v = z;
    } else if(abs(y + 1) < epsilon)
    {
        sideIndex = 3;
        u = x;
        v = z;
    } else if(abs(z - 1) < epsilon)
    {
        sideIndex = 4;
        u = x;
        v = y;
    } else if(abs(z + 1) < epsilon)
    {
        sideIndex = 5;
        u = x;
        v = y;
    }

    // Convert domain of u,v from [-1,1] to [0,sideIndex]
    uint32_t tex_x = (uint32_t)(((u + 1.0f) * (0.5f)) * sideVectorLength);
    uint32_t tex_y = (uint32_t)(((v + 1.0f) * (0.5f)) * sideVectorLength);

    // Make sure we're in the domain
    if(tex_x < 0)
        tex_x = 0;
    if(tex_y < 0)
        tex_y = 0;  
    if(tex_x >= sideVectorLength)
        tex_x = sideVectorLength - 1;
    if(tex_y >= sideVectorLength)
        tex_y = sideVectorLength - 1; 

    // Return the final coordinate
    return  (sideIndex * sideVectorLength * sideVectorLength) 
        * (tex_y * sideVectorLength) + tex_x;
}


template <typename T>
bool CoordContainerSkybox<T>::SetPoint(T x, T y, T z, uint32_t point)
{
    mtx.lock();
    uint32_t index = CartesianToIndex(x, y, z);
    points[index] = point;
    mtx.unlock();
}

/*
template<typename T>
Point3d<T> CoordContainerSkybox<T>::GetClosestPointCartesian(T azim, T evel)
{

}

template<typename T>
Point3d<T> CoordContainerSkybox<T>::GetClosestPointCartesian(T x, T y, T z)
{

}

template<typename T>
PointSphere<T> CoordContainerSkybox<T>::GetClosestPointSpherical(T x, T y, T z)
{
    
}

template<typename T>
PointSphere<T> CoordContainerSkybox<T>::GetClosestPointSpherical(T azim, T evel)
{

}
*/

template<typename T>
uint32_t CoordContainerSkybox<T>::GetClosestPixel(T azim, T evel)
{
    // Convert to cartesian
    Eigen::Vector3<T> point = SphericalToCartesian({azim, evel});

    // Grab absolute values and furthest coordinate value from zero.
    Eigen::Vector3<T> pointAbs(
        fabs(cartPoint.x), 
        fabs(cartPoint.y), 
        fabs(cartPoint.z)
    );
    constexpr float largestVal = pointAbs.maxCoeff();

    // Bounds checking (prevent divide by zero error)
    if(largestVal == 0)
        return 0;

    // Furtherest coordinate from zero should be the first to intersect 
    // with one side of the unit cube. We need to divide by abs since
    // negatives will cancel out.
    float targetLen = 1.0f;
    if(largestVal == pointAbs.x()) {
        targetLen = 1.0f / pointAbs.x();
    } else if(largestVal == pointAbs.y()) {
        targetLen = 1.0f / pointAbs.y();
    } else {
        targetLen = 1.0f / pointAbs.z();
    }

    // We'll just scale the unit vector until it hits that side.
    Eigen::Vector3<T> scaledPoint = targetLen * point;

    // And grab the corresponding pixel
    mtx.lock();
    uint32_t index = CartesianToIndex(scaledPoint.x(), scaledPoint.y(), scaledPoint.z());
    uint32_t data = points[index];
    mtx.unlock();

    return data;
}


