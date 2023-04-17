#include "SkydomeProjection.h"
#include "../Coordinates/CoordContainerHemiSpherical.h"

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
    // Grab the total size along with doing some sanity checks
    int vectorSideLen = 0;
    if(topImage != nullptr)
    {
        vectorSideLen = topImage->GetWidth();
        if(topImage->GetWidth() != topImage->GetHeight())
            throw std::range_error("topImage width must equal height!");
    }
    if(bottomImage != nullptr)
    {
        vectorSideLen = bottomImage->GetWidth();
         
        if(bottomImage->GetWidth() != bottomImage->GetHeight())
            throw std::range_error("bottomImage width must equal height!");
        if(topImage != nullptr && topImage->GetWidth() != bottomImage->GetWidth())
            throw std::range_error("bottomImage dimensions must equal topImage dimensions!");

    }

    std::shared_ptr<CoordContainerHemiSpherical<T>> sphereCoords = 
        std::make_shared<CoordContainerHemiSpherical<T>>(vectorSideLen);

    for(uint32_t i = 0; i < vectorSideLen; i++) {
        for(uint32_t j = 0; j < vectorSideLen; j++) {
            if(topImage != nullptr) {
                uint32_t topData = topImage->GetPixel(i, j);
                sphereCoords->SetPointDirect(TopHemiSurf, i, j, topData);
            }
            if(bottomImage != nullptr) {
                uint32_t bottomData = bottomImage->GetPixel(i, j);
                sphereCoords->SetPointDirect(BottomHemiSurf, i, j, bottomData);
            }
        }
    }


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
            T dist = sqrtf(u*u + v*v);
            T angle = atan2f(v, u);

            // We only care if the dist <= pi
            if(dist > pi)
                continue;
            
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