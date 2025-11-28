#include "CoordContainerSpherical.h"
#include "CoordConversions.h"

#include <iostream>

using namespace std;
using namespace EnvProj;

const float pi = 3.14159265358979323846f;
 
template<typename T>
uint32_t CoordContainerSpherical<T>::GetFinalIndex(uint32_t tex_x, uint32_t tex_y)
{
    // Constrain to range
    if(tex_x < 0)
        tex_x = 0;
    if(tex_x >= azimVectorSize)
        tex_x = azimVectorSize - 1;
    if(tex_y < 0)
        tex_y = 0;
    if(tex_y >= evelVectorSize)
        tex_y = evelVectorSize - 1;
        

    uint32_t finalIndex = (tex_y * azimVectorSize + tex_x);
    if(finalIndex < 0 || finalIndex >= points.size())
        throw std::range_error("Index is outside the range of size of array!");

    return finalIndex;
}

template<typename T>
uint32_t CoordContainerSpherical<T>::AzimElevToIndex(T azim, T evel)
{
    // Bind to from domains [-pi, pi], [-pi/2,pi/2]
    // to [0, 2pi], [0,pi]
    T fixedazim = azim + (T)pi;
    T fixedevel = evel + (T)pi/(T)2.0f;

    // Convert azim/evel to coordinates
    uint32_t x = (unsigned int)(((fixedazim) / (T)(2*pi)) * (T)azimVectorSize);
    uint32_t y = (unsigned int)(((fixedevel) / (T)(pi)) * (T)evelVectorSize);

    uint32_t finalIndex = GetFinalIndex(x, y);
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
bool CoordContainerSpherical<T>::SetPointDirect(uint32_t azim, uint32_t evel, uint32_t data)
{
    mtx.lock();
    uint32_t finalIndex = GetFinalIndex(azim, evel);
    points[finalIndex] = data;
    mtx.unlock();

    return true;
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
    PointSphere<T> pointSphere = CoordConversions<T>::CartesianToSpherical({x, y, z});
    return GetClosestPixel(pointSphere.azimuth, pointSphere.elevation);
}

namespace EnvProj {
    template class CoordContainerSpherical<float>;
    template class CoordContainerSpherical<double>;
}
