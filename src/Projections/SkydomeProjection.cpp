#include "SkydomeProjection.h"
#include "../Coordinates/CoordContainerSpherical.h"

using namespace EnvProj;
using namespace std;

const float pi = 3.14159265358979323846f;

template <typename T>
SkydomeProjection<T>::SkydomeProjection()
{

}

template <typename T>
SkydomeProjection<T>::~SkydomeProjection()
{

}

template <typename T>
std::shared_ptr<CoordContainerBase<T>> 
    SkydomeProjection<T>::LoadImageToSphericalCoords(EnvMapImage* topImage, EnvMapImage* bottomImage)
{
    //int totalSize = 0;
    //(topImage->GetHeight() * topImage->GetWidth());

    std::shared_ptr<CoordContainerSpherical<T>> sphereCoords = 
    std::make_shared<CoordContainerSpherical<T>>(20, 20);


    // TODO: Implement this


    // I'll have to check if this ref-counts correctly...
    std::shared_ptr<CoordContainerBase<T>> ptrBase = sphereCoords;
    return ptrBase;
}

template <typename T>
std::array<EnvMapImage,2> SkydomeProjection<T>::ConvertToImages(CoordContainerBase<T>* coords,
    uint32_t imageSideLength)
{
    // TODO: We should error check that coords actually has points?
    std::array<EnvMapImage,2> skydomeImages = {
        EnvMapImage(imageSideLength, imageSideLength),
        EnvMapImage(imageSideLength, imageSideLength)
    };

    // We're going to write to the top and bottom image at the same time.
    for(int i = 0; i < imageSideLength; i++)
    {
        for(int j = 0; j < imageSideLength; j++)
        {

            // Convert from [0, imageSideLength] to [-imageSideLength/2, imageSideLength/2]
            int offset_i = i - (imageSideLength / 2);
            int offset_j = j - (imageSideLength / 2);

            // Convert to domain [-pi, pi]
            T u = (((T)offset_i) / (T)imageSideLength) * pi;
            T v = (((T)offset_j) / (T)imageSideLength) * pi;

            // Get distance and angle of resultant vector
            T dist = u*u + v*v;
            T angle = atan2f(v, u);

            // We only care if the dist^2 <= pi^2
            if(angle >= (pi*pi))
                continue;

            // Do the expensive sqrtf only after we know we need this.
            dist = sqrtf(dist);
            
            // Distance is our elevation coordinate.
            // Angle is our Azimuth coordinate
            uint32_t pixelDataTop = coords->GetClosestPixel(angle, dist - pi/2);
            uint32_t pixelDataBottom = coords->GetClosestPixel(angle, dist + pi/2);

            // Set pixel in the final UV image
            skydomeImages[0].SetPixel(i, j, pixelDataTop);
            skydomeImages[1].SetPixel(i, j, pixelDataBottom);
        }
    }

    return skydomeImages;
}

template <typename T>
PointSphere<T> SkydomeProjection<T>::UVToSpherical(T u, T v)
{
    PointSphere<T> newPoint;
    newPoint.azimuth = (u * 2.0f * pi) - pi;
    newPoint.elevation = (v * pi) - (pi/2);

    //std::cout << newPoint.azimuth << "," << newPoint.elevation << std::endl;
    return newPoint;
}