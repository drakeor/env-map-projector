#include "CoordContainerSkybox.h"
#include "CoordConversions.h"
#include "Point3d.h"

#include <iostream>
#include <cmath>
#include <Eigen/Dense>

using namespace std;
using namespace EnvProj;

using Eigen::Vector3;

template <typename T>
CoordContainerSkybox<T>::CoordContainerSkybox(uint32_t _sideVectorLength)
    : points(_sideVectorLength * _sideVectorLength * 6, 0)
{
    std::cout << "Allocating skybox container with length " 
        << points.size() << std::endl;

#ifdef DEBUG_PRINT_COORD_CONTAINER_SKYBOX
    std::cout << "Allocating container with length " 
        << points.size() << std::endl;
#endif

    sideVectorLength = _sideVectorLength;
}

template <typename T>
uint32_t CoordContainerSkybox<T>::ToFinalIndex(uint32_t sideIndex, uint32_t tex_x, uint32_t tex_y)
{
    
    // Make sure we're in the domain for sideIndex
    if(sideIndex < 0) {
        std::cout << "sideIndex is " << sideIndex << std::endl;
        std::cout << "sideIndex escaped bounds 0>" << tex_x << std::endl;
        sideIndex = 0;
    }
    if(sideIndex >= 6) {
        std::cout << "sideIndex is " << sideIndex << std::endl;
        std::cout << "sideIndex escaped bounds 6<=" << tex_x << std::endl;
        sideIndex = 0;
    }

    // Make sure the tex_x and tex_y coordinates are within bounds.
    if(tex_x < 0) {
        std::cout << "tex_x escaped bounds 0>" << tex_x << std::endl;
        tex_x = 0;
    }
    if(tex_y < 0) {
        std::cout << "tex_y escaped bounds 0>" << tex_y << std::endl;
        tex_y = 0;  
    }
    if(tex_x >= sideVectorLength) {
        if(tex_x > sideVectorLength) {
            std::cout << "tex_x escaped bounds " << tex_x << ">" << sideVectorLength << std::endl;
        }
        tex_x = sideVectorLength - 1;
    }
    if(tex_y >= sideVectorLength) {
        if(tex_y > sideVectorLength) {
            std::cout << "tex_y escaped bounds " << tex_y << ">" << sideVectorLength << std::endl;
        }
        tex_y = sideVectorLength - 1; 
    }

    uint32_t finalIndex = (sideIndex * sideVectorLength * sideVectorLength) 
        + (tex_y * sideVectorLength) + tex_x;
        
    if(finalIndex < 0 || finalIndex >= points.size()) {
        std::cout << "Attempting to access index " << finalIndex << std::endl;
        std::cout << "Size of array is " << points.size() << std::endl;
        throw std::range_error("Index is outside the range of size of array!");
    }

    return finalIndex;
}

// Note that the cartesian point is expected to be a point on the unit cube!
template <typename T>
uint32_t CoordContainerSkybox<T>::CartesianToIndex(T x, T y, T z)
{
    // Define some epsilon value for floating point comparisons
    T epsilon = 10e-9f;
    
    std::array<T, 3> cartCoord = {x, y, z};
    //Eigen::Vector3<T> cartCoord(x, y, z);
    uint32_t sideIndex = 0;
    T u = 0;
    T v = 0;

    // Convert cartesian point to a side
    for(uint32_t i = 0; i < 6; i++)
    {
        Point3d<int32_t> coordMap = CoordConversions<T>::SideToCoordMapInv(
            static_cast<SkyboxSurf>(i));
        T constVal = CoordConversions<T>::SideToConstVal(
            static_cast<SkyboxSurf>(i));
        
        std::array<T, 3> tmpUvCoord = {0, 0, 0};
        //Eigen::Vector3<T> tmpUvCoord(0, 0, 0);

        tmpUvCoord[0] = cartCoord[coordMap.x];
        tmpUvCoord[1] = cartCoord[coordMap.y];
        tmpUvCoord[2] = cartCoord[coordMap.z];

        if(fabs(tmpUvCoord[2] - constVal) < epsilon)
        {
            sideIndex = i;
            u = tmpUvCoord[0];
            v = tmpUvCoord[1];
            break;
        }

        /*
        tmpUvCoord(0) = cartCoord(coordMap.x);
        tmpUvCoord(1) = cartCoord(coordMap.y);
        tmpUvCoord(2) = cartCoord(coordMap.z);

        if(fabs(tmpUvCoord(2) - constVal) < epsilon)
        {
            sideIndex = i;
            u = tmpUvCoord(0);
            v = tmpUvCoord(1);
            break;
        }*/

    }

    // Convert domain of u,v from [-1,1] to [0,sideIndex]
    T u_scaled = ((u + 1.0f) * (0.5f));
    T v_scaled = ((v + 1.0f) * (0.5f));
    uint32_t tex_x = (uint32_t)(u_scaled * sideVectorLength);
    uint32_t tex_y = (uint32_t)(v_scaled * sideVectorLength);


    // Return the final coordinate
#ifdef DEBUG_PRINT_COORD_CONTAINER_SKYBOX
    std::cout << "Side: " << sideIndex << "; Tex: " << tex_x
        << "," << tex_y << std::endl;
#endif

    
    uint32_t finalIndex = ToFinalIndex(sideIndex, tex_x, tex_y);
    return finalIndex;
}


template <typename T>
bool CoordContainerSkybox<T>::SetPoint(T x, T y, T z, uint32_t point)
{
    mtx.lock();
    uint32_t index = CartesianToIndex(x, y, z);
    points[index] = point;
    mtx.unlock();
    
#ifdef DEBUG_PRINT_COORD_CONTAINER_SKYBOX
    std::cout << "Set Point: " << index << std::endl;
#endif

    return true;
}

template <typename T>
bool CoordContainerSkybox<T>::SetPointDirect(SkyboxSurf surf, T tex_x, T tex_y, uint32_t point)
{
    mtx.lock();
    
    uint32_t finalIndex = ToFinalIndex(static_cast<uint32_t>(surf), tex_x, tex_y);

    if(finalIndex < 0 || finalIndex >= points.size()) {
        std::cout << "SetPointDirect Attempting to access index " << finalIndex << std::endl;
        std::cout << "Size of array is " << points.size() << std::endl;
        throw std::range_error("Index is outside the range of size of array!");
    }
    points[finalIndex] = point;

    mtx.unlock();

    return true;
}


template<typename T>
uint32_t CoordContainerSkybox<T>::GetClosestPixel(T x, T y, T z)
{
    // Convert to cartesian
    Eigen::Vector3<T> point(x, y, z);

    // Grab absolute values and furthest coordinate value from zero.
    Eigen::Vector3<T> pointAbs(
        fabs(point.x()), 
        fabs(point.y()), 
        fabs(point.z())
    );
    T largestVal = pointAbs.maxCoeff();

    // Bounds checking (prevent divide by zero error)
    if(largestVal == 0)
        return 0;

    // Furtherest coordinate from zero should be the first to intersect 
    // with one side of the unit cube. We need to divide by abs since
    // negatives will cancel out.
    T targetLen = 1.0f;
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

#ifdef DEBUG_PRINT_COORD_CONTAINER_SKYBOX
    std::cout << "Translated Point: " 
        << scaledPoint.x() << ", "
        << scaledPoint.y() << ", "
        << scaledPoint.z() << std::endl;

    std::cout << "Get Point: " << index << std::endl;
#endif

    return data;
}


template<typename T>
uint32_t CoordContainerSkybox<T>::GetClosestPixel(T azim, T evel)
{
    // Convert to cartesian and pass to above function
    Point3d<T> point = CoordConversions<T>::SphericalToCartesian({azim, evel});
    return GetClosestPixel(point.x, point.y, point.z);
}

namespace EnvProj {
    template class CoordContainerSkybox<float>;
    template class CoordContainerSkybox<double>;
}
