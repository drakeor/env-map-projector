#include "SkyboxProjection.h"

using namespace Eigen;

SkyboxProjection::SkyboxProjection()
{

}

SkyboxProjection::~SkyboxProjection()
{
    
}

void SkyboxProjection::LoadImageToSphericalCoords(
        EnvMapImage* topImage, EnvMapImage* bottomImage, 
        EnvMapImage* leftImage, EnvMapImage* rightImage,
        EnvMapImage* frontImage, EnvMapImage* backImage)
{
    coords.Empty();
#ifdef STORE_CARTESIAN_COORDS
    coordsCart.empty();
#endif

        // Defines how coordinates are oriented with respect to (u,v) coordinates.
        // This allows us to adapt to whatever direction the coordinates are.
        // The identity matrix represents standard mathematical configuration.
        // The v coordinate is flipped, so we flip the z coordinate in response
        Eigen::Matrix3f D;
        D << 1, 0, 0,
            0, 1, 0,
            0, 0, -1;

    // Lambda function to convert uv position [0,1] to a coordinates 
    // on one of the sides [-1,1]. Note that we treat the skybox as a unit cube.
    Eigen::Vector3f one(1.0f, 1.0f, 1.0f);
    auto f = [=](Eigen::Vector3f i) {
        return 2*i - one;
    };
    // Inverse of above function. (For reference later)
    auto f_inv = [=](Eigen::Vector3f i) {
        return 0.5f*i + 0.5f*one;
    };

    // Converting UV to coords for a particular side
    auto uvToCoord = [&](Eigen::Matrix3f Pmat, float offset, EnvMapImage* image) {
        
        for(int i = 0; i < image->GetWidth(); i++)
        {
            for(int j = 0; j < image->GetHeight(); j++)
            {
                unsigned int pixelData = image->GetPixel(i, j);
                float u = (float)i / (float)image->GetWidth();
                float v = (float)j / (float)image->GetHeight();

                Eigen::Vector3f X(u, v, 0);
                Eigen::Vector3f p = f(Pmat*D*X);

                // Crappy way fix a certain axis to the offset amount
                if(Pmat(0, 2) == 1)
                    p(0) = offset;
                if(Pmat(1, 2) == 1)
                    p(1) = offset;
                if(Pmat(2, 2) == 1)
                    p(2) = offset;

                //std::cout << p.z() << "," << std::endl;
                std::cout << "(" << p.x() << "," << p.y() << "," << p.z() << "), ";
#ifdef STORE_CARTESIAN_COORDS
                coordsCart.push_back(p);
#endif
                // TODO: Convert cartesian to spherical coordinates and store.
            }
            std::cout << std::endl;
        }
    };


    // Go through each image and store the result
    // Convert f:[u,v,1] -> [x,y,z]

    // Start with the top image and bottom images
    {
        // Top-Bottom plane is simple xy plane.
        Eigen::Matrix3f P1;
            P1 << 1, 0, 0,
                0, 1, 0,
                0, 0, 1;
        uvToCoord(P1, 1, topImage);
        uvToCoord(P1, -1, bottomImage);
    }

    // Do the front and back images
    {
        // Front-Back is the yz plane
        Eigen::Matrix3f P2;
            P2 << 0, 0, 1,
                1, 0, 0,
                0, 1, 0;
        uvToCoord(P2, 1, backImage);
        //uvToCoord(P2, -1, frontImage);
    }

    // Do the left and right images
    {
        // Left-Right is the xz plane
        Eigen::Matrix3f P3;
            P3 << 1, 0, 0,
                0, 0, 1,
                0, 1, 0;
        uvToCoord(P3, 1, rightImage);
        uvToCoord(P3, -1, leftImage);
    }

}

EnvMapImage ConvertToImageTop(int width, int height)
{
    // Only output the top image
    EnvMapImage newImage(width, height);
    return newImage;
}