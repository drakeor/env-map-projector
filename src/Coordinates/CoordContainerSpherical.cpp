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

    uint32_t finalIndex = (y* azimVectorSize + x);
    if(finalIndex < 0 || finalIndex >= points.size())
        throw std::range_error("Index is outside the range of size of array!");

    return finalIndex;
}

template<typename T>
CoordContainerSpherical<T>::CoordContainerSpherical(unsigned int _azimVectorSize, unsigned int _evelVectorSize)
    : points(_azimVectorSize * _evelVectorSize, 0)
{
    azimVectorSize = _azimVectorSize;
    evelVectorSize = _evelVectorSize;
#ifdef DEBUG_PRINT_COORD_CONTAINER_SPHERICAL
        std::cout << "Allocating container with length " 
        << points.size() << std::endl;
#endif
}

template<typename T>
bool CoordContainerSpherical<T>::SetPoint(T azim, T evel, uint32_t data)
{
    mtx.lock();
    auto i = AzimElevToIndex(azim, evel);
    points[i] = data;
    mtx.unlock();

#ifdef DEBUG_PRINT_COORD_CONTAINER_SPHERICAL
    std::cout << "Set Point " 
        << i << std::endl;
#endif

    return true;
}

template<typename T>
uint32_t CoordContainerSpherical<T>::GetClosestPixel(T azim, T evel)
{
    // Grab point directly
    mtx.lock();
    auto i = AzimElevToIndex(azim, evel);
    auto point =  points[i];
    mtx.unlock();

#ifdef DEBUG_PRINT_COORD_CONTAINER_SPHERICAL
    std::cout << "Get Point " 
        << i << std::endl;
#endif

    return point;
}

template<typename T>
uint32_t CoordContainerSpherical<T>::GetClosestPixel(T x, T y, T z)
{
    // Convert point to spherical and pass to above function
    Eigen::Vector3<T> pointCart(x, y, z);
    PointSphere<T> pointSphere = CoordConversions<T>::CartesianToSpherical(pointCart);
    return GetClosestPixel(pointSphere.azimuth, pointSphere.elevation);
}


