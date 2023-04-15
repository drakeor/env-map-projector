#include "EquirectangularProjection.h"

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

    std::shared_ptr<SphereCoordsContainer<T>> sphereCoords = 
        std::make_shared<SphereCoordsContainer<T>>(totalSize);

    for(int i = 0; i < image->GetWidth(); i++)
    {
        for(int j = 0; j < image->GetHeight(); j++)
        {
            // Convert to UV coords. Domain: [0,1]
            float u = (float)i / (float)(image->GetWidth()-1);
            float v = (float)j / (float)(image->GetHeight()-1);
            uint32_t pixelData = image->GetPixel(i, j);

            // Coordinates are stored internally as spherical coordinates
            // Convert and add to our coordinate collection
            PointSphere<T> sphericalPt = UVToSpherical(u, v);
            sphereCoords->SetPoint(sphericalPt.azimuth, sphericalPt.elevation, pixelData);
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
    newPoint.azimuth = (inputPt.x * 2.0f * pi) - pi;
    newPoint.elevation = (inputPt.y * pi) - (pi/2);

    return newPoint;
}

