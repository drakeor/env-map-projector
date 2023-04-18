#include "HemisphericalProjection.h"
#include "../Coordinates/CoordContainerHemiSpherical.h"

using namespace EnvProj;
using namespace std;

const float pi = 3.14159265358979323846f;

template <typename T>
HemisphericalProjection<T>::HemisphericalProjection()
{

}

template <typename T>
HemisphericalProjection<T>::~HemisphericalProjection()
{

}

template <typename T>
std::shared_ptr<CoordContainerBase<T>> 
    HemisphericalProjection<T>::LoadImageToSphericalCoords(EnvMapImage* topImage, EnvMapImage* bottomImage)
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
                // For whatever reason, we store the top to bottom and vice versa when loading.
                sphereCoords->SetPointDirect(BottomHemiSurf, i, j, topData);
            }
            if(bottomImage != nullptr) {
                uint32_t bottomData = bottomImage->GetPixel(i, j);
                // For whatever reason, we store the top to bottom and vice versa when loading.
                sphereCoords->SetPointDirect(TopHemiSurf, i, j, bottomData);
            }
        }
    }


    // I'll have to check if this ref-counts correctly...
    std::shared_ptr<CoordContainerBase<T>> ptrBase = sphereCoords;
    return ptrBase;
}

template <typename T>
std::array<EnvMapImage,2> HemisphericalProjection<T>::ConvertToImages(CoordContainerBase<T>* coords,
    uint32_t imageSideLength)
{
    // TODO: We should error check that coords actually has points?
    std::array<EnvMapImage,2> skydomeImages = {
        EnvMapImage(imageSideLength, imageSideLength),
        EnvMapImage(imageSideLength, imageSideLength)
    };

    // We're going to write to the top and bottom image at the same time.
    // This is easiest with cartesian coordinates
    for(int i = 0; i < imageSideLength; i++)
    {
        for(int j = 0; j < imageSideLength; j++)
        {
            // Convert from [0, imageSideLength] to [0, 1]
            T x_coord = (T)i / ((T)imageSideLength - 1.0f);
            T y_coord = (T)j / ((T)imageSideLength - 1.0f);

            // Convert from [0, 1] to [-1, 1]
            x_coord = 2.0f*(x_coord - 0.5f);
            y_coord = 2.0f*(y_coord - 0.5f);

            // Only worry about getting coordinates and setting pixels
            // if the L2 <= 1
            T len_squared = x_coord*x_coord + y_coord*y_coord;
            if(len_squared > 1.0f) {
                skydomeImages[0].SetPixel(i, j, 0x00);
                skydomeImages[1].SetPixel(i, j, 0x00);
                continue;
            }

            // Derive z coordinate by re-arranging the equation for 
            // a unit sphere.
            T z_coord = sqrtf(1.0f - len_squared);

            //std::cout << len_squared << "," << z_coord << std::endl;

            uint32_t pixelDataTop = coords->GetClosestPixel(x_coord, y_coord, -z_coord);
            uint32_t pixelDataBottom = coords->GetClosestPixel(x_coord, y_coord, z_coord);

            // Set pixel in the final UV image
            skydomeImages[0].SetPixel(i, j, pixelDataTop);
            skydomeImages[1].SetPixel(i, j, pixelDataBottom);
        }
    }

    return skydomeImages;
}

/*
template <typename T>
PointSphere<T> HemisphericalProjection<T>::UVToSpherical(T u, T v)
{
    PointSphere<T> newPoint;
    newPoint.azimuth = (u * 2.0f * pi) - pi;
    newPoint.elevation = (v * pi) - (pi/2);

    //std::cout << newPoint.azimuth << "," << newPoint.elevation << std::endl;
    return newPoint;
}
*/