#include "CoordContainerSkybox.h"

using namespace std;
using namespace EnvProj;

template <typename T>
CoordContainerSkybox<T>::CoordContainerSkybox(unsigned int _sideVectorLength)
{
    sideVectorLength = _sideVectorLength;
    points.reserve(_sideVectorLength * _sideVectorLength * 6);
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
    unsigned int tex_x = (unsigned int)(((u + 1.0f) * (0.5f)) * sideVectorLength);
    unsigned int tex_y = (unsigned int)(((v + 1.0f) * (0.5f)) * sideVectorLength);

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
    return (tex_y* sideVectorLength + x) * sideIndex;
}


template <typename T>
bool CoordContainerSkybox<T>::SetPoint(T x, T y, T z, Point3d<T> point)
{

}

template<typename T>
Point3d<T> CoordContainerSkybox<T>::GetClosestPointCartesian(T azim, T evel)
{

}

template<typename T>
Point3d<T> CoordContainerSkybox<T>::GetClosestPointCartesian(T x, T y, T z)
{

}

template<typename T>
PointSphere<T> CoordContainerSkybox<T>::GetClosestPointSpherical(T azim, T evel)
{
    
}

template<typename T>
PointSphere<T> CoordContainerSkybox<T>::GetClosestPointSpherical(T x, T y, T z)
{
    
}