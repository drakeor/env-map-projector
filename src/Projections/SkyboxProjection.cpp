#include "SkyboxProjection.h"
#include <memory>

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

    // Preallocate an array big enough to hold the data
    // from all six images.
    int totalSize = 0;
    if(topImage != nullptr)
        totalSize += (topImage->GetHeight() * topImage->GetWidth());
    if(bottomImage != nullptr)
        totalSize += (bottomImage->GetHeight() * bottomImage->GetWidth());
    if(leftImage != nullptr)
        totalSize += (leftImage->GetHeight() * leftImage->GetWidth());
    if(rightImage != nullptr)
        totalSize += (rightImage->GetHeight() * rightImage->GetWidth());
    if(frontImage != nullptr)
        totalSize += (frontImage->GetHeight() * frontImage->GetWidth());
    if(backImage != nullptr)
        totalSize += (backImage->GetHeight() * backImage->GetWidth());

    std::cout << "Attempting to allocate coordinate container of size "
        << totalSize << std::endl;

    std::shared_ptr<CoordContainerSkybox<T>> coords = 
        std::make_shared<CoordContainerSkybox<T>>(totalSize);

    // Lambda function to convert uv position [0,1] to a coordinates 
    // on one of the sides [-1,1]. Note that we treat the skybox as a unit cube.
    auto f = [=](float i) {
        return 2*i - 1;
    };
    // Inverse of above function. (For reference later)
    auto f_inv = [=](float i) {
        return 0.5f*i + 0.5f*1;
    };

    // Converting UV to coords for a particular side
    auto uvToCoord = [&](Eigen::Vector3i coordMap, float constVal, EnvMapImage* image) {
        
        // We only need to build this matrix once
        for(int i = 0; i < image->GetWidth(); i++)
        {
            for(int j = 0; j < image->GetHeight(); j++)
            {
                unsigned int pixelData = image->GetPixel(i, j);
                float u = (float)i / (float)(image->GetWidth()-1);
                float v = (float)j / (float)(image->GetHeight()-1);

                // Convert from domain [0,1] to [-1,1] on u and v and add in the constant value
                Eigen::Vector3f uvCoord(f(u), f(v), constVal);

                // Transform uv to cartesian coordinates
                // Convert f:[u,v,c] -> [x,y,z]
                Eigen::Vector3f cartCoord;
                cartCoord(0) = uvCoord(coordMap[0]);
                cartCoord(1) = uvCoord(coordMap[1]);
                cartCoord(2) = uvCoord(coordMap[2]);

                // Lastly, convert to spherical coordinates and store.
                coords->SetPoint(cartCoord.x(), cartCoord.y(), cartCoord.z(), pixelData);
            }
        }
    };


    // Go through each image and store the result

    // Start with the top image and bottom images
    if(topImage != nullptr)
        uvToCoord(SideToCoordMap(TopSurf), SideToConstVal(TopSurf), topImage);
    if(bottomImage != nullptr)
        uvToCoord(SideToCoordMap(BottomSurf), SideToConstVal(BottomSurf), bottomImage);

    // Do the front and back images
    if(backImage != nullptr)
        uvToCoord(BackSurf), SideToConstVal(BackSurf), backImage);
    if(frontImage != nullptr)
        uvToCoord(FrontSurf), SideToConstVal(FrontSurf), frontImage);

    // Do the left and right images
    if(rightImage != nullptr)
        uvToCoord(RightSurf), SideToConstVal(RightSurf), rightImage);
    if(leftImage != nullptr)
        uvToCoord(LeftSurf), SideToConstVal(LeftSurf), leftImage);

    std::shared_ptr<CoordContainerBase<T>> ptrBase = sphereCoords;
    return ptrBase;
}


template <typename T>
std::array<EnvMapImage, 6> SkyboxProjection<T>::ConvertToImages(CoordContainerBase<T>* coords,
    uint32_t width, uint32_t height)
{
    // Build array of six images
    std::array<EnvMapImage, 6> skyboxImgs = { 
        EnvMapImage(width, height), EnvMapImage(width, height),
        EnvMapImage(width, height), EnvMapImage(width, height),
        EnvMapImage(width, height), EnvMapImage(width, height) 
    };

    // Output all six images
    for(int k = 0; k < skyboxImgs.size(); k++)
    {
        Eigen::Vector3i coordMap = SideToCoordMap(static_cast<SkyboxSurf>(k)),
        float constVal = SideToConstVal(static_cast<SkyboxSurf>(k));

        // We only need to build this matrix once
        for(int i = 0; i < image->GetWidth(); i++)
        {
            for(int j = 0; j < image->GetHeight(); j++)
            {
                float u = (float)i / (float)(image->GetWidth()-1);
                float v = (float)j / (float)(image->GetHeight()-1);

                // Convert from domain [0,1] to [-1,1] on u and v and add in the constant value
                Eigen::Vector3f uvCoord(f(u), f(v), constVal);

                // Transform uv to cartesian coordinates
                // Convert f:[u,v,c] -> [x,y,z]
                Eigen::Vector3f cartCoord;
                cartCoord(0) = uvCoord(coordMap[0]);
                cartCoord(1) = uvCoord(coordMap[1]);
                cartCoord(2) = uvCoord(coordMap[2]);
                
                unsigned int pixelData = coords->GetClosestPixel(cartCoord.x(), cartCoord.y(), cartCoord.z());
                
                // Lastly, convert to spherical coordinates and store.
                skyboxImgs[k].SetPixel(i, j, pixelData);
            }
        }
    }

    return skyboxImgs;
}

template<typename T>
T SkyboxProjection<T>::SideToConstVal(SkyboxSurf side)
{
    switch(side)
    {
        // These are on positive axis
        case BottomSurf:
        case BackSurf:
        case RightSurf:
            return 1.0f;

        // These are on negative axis
        case TopSurf:
        case LeftSurf:
        case FrontSurf:
            return -1.0f;
    }

    return 1.0f;
}

template<typename T>
Eigen::Vector3i SkyboxProjection<T>::SideToCoordMap(SkyboxSurf side)
{
    switch(side)
    {
        // Top-Bottom plane is simple xy plane. z = {-1, 1}.
        case TopSurf:
        case BottomSurf:
            return Eigen::Vector3i(0, 1, 2);

        // Left-Right is the xz plane. y = {-1, 1}.
        case LeftSurf:
        case RightSurf:
            return Eigen::Vector3i(0, 2, 1);

        // Front-Back is the yz plane. x = {-1, 1}.
        case FrontSurf:
        case BackSurf:
            return Eigen::Vector3i(2, 0, 1);
    }

    return Eigen::Vector3i(0, 1, 2);
}