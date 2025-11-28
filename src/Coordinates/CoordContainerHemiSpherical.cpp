#include "CoordContainerHemiSpherical.h"
#include "CoordConversions.h"
#include "Point3d.h"

#include <iostream>

using namespace std;
using namespace EnvProj;

const float pi = 3.14159265358979323846f;
 
template<typename T>
uint32_t CoordContainerHemiSpherical<T>::GetFinalIndex(HemisphereSurf surf, uint32_t tex_x, uint32_t tex_y)
{
    // Constrain to range
    uint32_t surfIndex = static_cast<uint32_t>(surf);
    if(surfIndex < 0)
        surfIndex = 0;
    if(surfIndex >= 2)
        surfIndex = 1;

    if(tex_x < 0)
        tex_x = 0;
    if(tex_x >= vectorSize)
        tex_x = vectorSize - 1;
    if(tex_y < 0)
        tex_y = 0;
    if(tex_y >= vectorSize)
        tex_y = vectorSize - 1;

#ifdef DEBUG_PRINT_COORD_CONTAINER_HEMISPHERICAL
        std::cout << "Final Index for (" << tex_x << "," << tex_y 
            << "). Surface: " << surfIndex << std::endl; 
#endif


    uint32_t finalIndex = (surfIndex * vectorSize * vectorSize) 
        + (tex_y * vectorSize) 
        + tex_x;
    if(finalIndex < 0 || finalIndex >= points.size())
        throw std::range_error("Index is outside the range of size of array!");

    return finalIndex;
}

template<typename T>
CoordContainerHemiSpherical<T>::CoordContainerHemiSpherical(uint32_t _vectorSize)
    : points(_vectorSize * _vectorSize * 2, 0)
{
    vectorSize = _vectorSize;
#ifdef DEBUG_PRINT_COORD_CONTAINER_HEMISPHERICAL
        std::cout << "Allocating container with length " 
        << points.size() << std::endl;
#endif
}

template<typename T>
bool CoordContainerHemiSpherical<T>::SetPointDirect(HemisphereSurf surf, uint32_t tex_x, uint32_t tex_y, uint32_t data)
{
    mtx.lock();
    uint32_t finalIndex = GetFinalIndex(surf, tex_x, tex_y);
    points[finalIndex] = data;
    mtx.unlock();

#ifdef DEBUG_PRINT_COORD_CONTAINER_HEMISPHERICAL
    std::cout << "Set Point " 
        << finalIndex << std::endl;
#endif

    return true;
}

template<typename T>
uint32_t CoordContainerHemiSpherical<T>::GetClosestPixel(T azim, T elev)
{
    PointSphere<T> pointSphere;
    pointSphere.azimuth = azim;
    pointSphere.elevation = elev;
    
    Point3d<T> pointCart = CoordConversions<T>::SphericalToCartesian(pointSphere);
    return GetClosestPixel(pointCart.x, pointCart.y, pointCart.z);
}

template<typename T>
uint32_t CoordContainerHemiSpherical<T>::GetClosestPixel(T x, T y, T z)
{
    // z-coordinate gives us the top/bottom surface.
    HemisphereSurf surf = TopHemiSurf;
    if(z < 0)
        surf = BottomHemiSurf;

    // Convert domain from [-1,1] -> [0,1]
    T u = (x + 1.0f) * 0.5f;
    T v = (y + 1.0f) * 0.5f;

    // Convert from [0,1] -> [0, vectorSize]
    uint32_t tex_x = (uint32_t)roundf(u * vectorSize);
    uint32_t tex_y = (uint32_t)roundf(v * vectorSize);

    // Grab the point 
    mtx.lock();
    uint32_t finalIndex = GetFinalIndex(surf, tex_x, tex_y);
    auto point = points[finalIndex];
    mtx.unlock();

#ifdef DEBUG_PRINT_COORD_CONTAINER_HEMISPHERICAL
    std::cout << "Get Point " 
        << finalIndex << std::endl;
#endif

    return point;
}

namespace EnvProj {
    template class CoordContainerHemiSpherical<float>;
    template class CoordContainerHemiSpherical<double>;
}
