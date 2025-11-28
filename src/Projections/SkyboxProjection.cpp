#include "SkyboxProjection.h"
#include "../Coordinates/CoordContainerSkybox.h"
#include "../Coordinates/CoordConversions.h"

#include <memory>
#include <cmath>

using namespace Eigen;
using namespace EnvProj;


template<typename T>
SkyboxProjection<T>::SkyboxProjection()
{

}

template<typename T>
SkyboxProjection<T>::~SkyboxProjection()
{
    
}

template<typename T>
std::shared_ptr<CoordContainerBase<T>> SkyboxProjection<T>::LoadImageToSphericalCoords(
        EnvMapImage* topImage, EnvMapImage* bottomImage, 
        EnvMapImage* leftImage, EnvMapImage* rightImage,
        EnvMapImage* frontImage, EnvMapImage* backImage)
{

    // Both sanity check and get the length for the sides at the same time.
    auto sideLength = [=](EnvMapImage* image, int currentSideLen) {
        // Ignore non-loaded images
        if(image == nullptr)
            return currentSideLen;
        // If loaded, make sure its a valid length
        if(image->GetHeight() == 0) {
            throw std::range_error("Skybox images must have a non-zero size!");
        }
        // Make sure it's square
        if(image->GetHeight() != image->GetWidth()) {
            throw std::range_error("Skybox images must be square!");
        }
        // Make sure side length matches the other side lengths.
        if(currentSideLen != 0) {
            if(currentSideLen != image->GetHeight()) {
                throw std::range_error("All skybox images must be the same size!");
            }
        }
        return image->GetHeight();
    };

    // Grab the side length
    int totalSize = sideLength(topImage, 0);
    totalSize = sideLength(bottomImage, totalSize);
    totalSize = sideLength(leftImage, totalSize);
    totalSize = sideLength(rightImage, totalSize);
    totalSize = sideLength(frontImage, totalSize);
    totalSize = sideLength(backImage, totalSize);

    std::shared_ptr<CoordContainerSkybox<T>> coords = 
        std::make_shared<CoordContainerSkybox<T>>(totalSize);

    
    // Converting UV to coords for a particular side
    auto uvToCoord = [&](SkyboxSurf surf, EnvMapImage* image) {
        // We only need to build this matrix once
        for(int i = 0; i < image->GetWidth(); i++)
        {
            for(int j = 0; j < image->GetHeight(); j++)
            {
                unsigned int pixelData = image->GetPixel(i, j);
                coords->SetPointDirect(surf, i, j, pixelData);
            }
        }
    };


    // Go through each image and store the result

    // Start with the top image and bottom images
    if(topImage != nullptr) {
        uvToCoord(TopSurf, topImage);
    }
    if(bottomImage != nullptr)
        uvToCoord(BottomSurf, bottomImage);

    // Do the front and back images
    if(backImage != nullptr)
        uvToCoord(BackSurf, backImage);
    if(frontImage != nullptr)
        uvToCoord(FrontSurf, frontImage);

    // Do the left and right images
    if(rightImage != nullptr)
        uvToCoord(RightSurf, rightImage);
    if(leftImage != nullptr)
        uvToCoord(LeftSurf, leftImage);

    std::shared_ptr<CoordContainerBase<T>> ptrBase = coords;
    return ptrBase;
}

template <typename T>
std::array<EnvMapImage, 6> SkyboxProjection<T>::ConvertToImages(CoordContainerBase<T>* coords,
    uint32_t sideLength)
{
    // Build array of six images
    std::array<EnvMapImage, 6> skyboxImgs = { 
        EnvMapImage(sideLength, sideLength), EnvMapImage(sideLength, sideLength),
        EnvMapImage(sideLength, sideLength), EnvMapImage(sideLength, sideLength),
        EnvMapImage(sideLength, sideLength), EnvMapImage(sideLength, sideLength) 
    };

    // Output all six images
    for(int k = 0; k < skyboxImgs.size(); k++)
    {
        Point3d<int32_t> coordMap = CoordConversions<T>::SideToCoordMap(static_cast<SkyboxSurf>(k));
        T constVal = CoordConversions<T>::SideToConstVal(static_cast<SkyboxSurf>(k));

        for(int i = 0; i < skyboxImgs[k].GetWidth(); i++)
        {
            for(int j = 0; j < skyboxImgs[k].GetHeight(); j++)
            {
                T u = (T)i / (T)(skyboxImgs[k].GetWidth() - 1);
                T v = (T)j / (T)(skyboxImgs[k].GetHeight() - 1);

                // Convert uv position [0,1] to a coordinates 
                // on one of the sides [-1,1]. Note that we treat the skybox as a unit cube.
                u = 2.0f * u - 1.0f;
                v = 2.0f * v - 1.0f;

                // Convert from domain [0,1] to [-1,1] on u and v and add in the constant value
                std::array<T, 3> uvCoord = {u, v, constVal};
                //Eigen::Vector3f uvCoord(u, v, constVal);

                // Transform uv to cartesian coordinates
                // Convert f:[u,v,c] -> [x,y,z]
                //Eigen::Vector3f cartCoord;
                //std::array<T, 3> cartCoord = {0, 0, 0};
                Point3d<T> cartCoord;

                cartCoord.x = uvCoord[coordMap.x];
                cartCoord.y = uvCoord[coordMap.y];
                cartCoord.z = uvCoord[coordMap.z];

                T length = std::sqrt(cartCoord.x * cartCoord.x +
                    cartCoord.y * cartCoord.y + cartCoord.z * cartCoord.z);
                if(length > 0)
                {
                    cartCoord.x /= length;
                    cartCoord.y /= length;
                    cartCoord.z /= length;
                }

                uint32_t pixelData = coords->GetClosestPixel(cartCoord.x, cartCoord.y, cartCoord.z);

                /*cartCoord(0) = uvCoord(coordMap.x);
                cartCoord(1) = uvCoord(coordMap.y);
                cartCoord(2) = uvCoord(coordMap.z);
                
                uint32_t pixelData = coords->GetClosestPixel(cartCoord.x(), cartCoord.y(), cartCoord.z());
                */

                // Lastly, convert to spherical coordinates and store.
                skyboxImgs[k].SetPixel(i, j, pixelData);
            }
        }
    }

    return skyboxImgs;
}

template class SkyboxProjection<float>;
template class SkyboxProjection<double>;
