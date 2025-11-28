#include "EquirectangularProjection.h"
#include "../Coordinates/CoordContainerSpherical.h"

using namespace EnvProj;
using namespace std;

const float pi = 3.14159265358979323846f;

template <typename T>
EquirectangularProjection<T>::EquirectangularProjection()
{

}

template <typename T>
EquirectangularProjection<T>::~EquirectangularProjection()
{

}

template <typename T>
std::shared_ptr<CoordContainerBase<T>> 
    EquirectangularProjection<T>::LoadImageToSphericalCoords(
    EnvMapImage* image)
{
    int totalSize = (image->GetHeight() * image->GetWidth());

    std::cout << "Attempting to allocate coordinate container of size "
        << totalSize << std::endl;

    std::shared_ptr<CoordContainerSpherical<T>> sphereCoords = 
        std::make_shared<CoordContainerSpherical<T>>(image->GetWidth(), image->GetHeight());

    for(int i = 0; i < image->GetWidth(); i++)
    {
        for(int j = 0; j < image->GetHeight(); j++)
        {
            // SphereCoords is a direct mapping, so no conversion needed.
            uint32_t pixelData = image->GetPixel(i, j);
            sphereCoords->SetPointDirect(i, j, pixelData);
        }
    }
    
    // I'll have to check if this ref-counts correctly...
    std::shared_ptr<CoordContainerBase<T>> ptrBase = sphereCoords;
    return ptrBase;
}

template <typename T>
EnvMapImage EquirectangularProjection<T>::ConvertToImage(CoordContainerBase<T>* coords, 
    int width, int height)
{
    // TODO: We should error check that coords actually has points?

    EnvMapImage newImage(width, height);

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            // Convert to UV coordinates. Domain: [0,1]
            float u = (float)i / (float)(width-1);
            float v = (float)j / (float)(height-1);

            // Equirectangular is the easiest. Grab the spherical points.
            auto sphericalPt = UVToSpherical(u, v);
            uint32_t pointData = coords->GetClosestPixel(sphericalPt.azimuth,
                sphericalPt.elevation);
            unsigned int pixelData = pointData;

            // Set pixel in the final UV image
            newImage.SetPixel(i, j, pixelData);
        }
    }

    return newImage;
}

template<typename T>
PointSphere<T> EquirectangularProjection<T>::UVToSpherical(T u, T v)
{
    PointSphere<T> newPoint;
    newPoint.azimuth = (u * 2.0f * pi) - pi;
    newPoint.elevation = (v * pi) - (pi/2);

    //std::cout << newPoint.azimuth << "," << newPoint.elevation << std::endl;
    return newPoint;
}

namespace EnvProj {
    template class EquirectangularProjection<float>;
    template class EquirectangularProjection<double>;
}

