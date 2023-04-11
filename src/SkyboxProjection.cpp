#include "SkyboxProjection.h"

using namespace Eigen;

SkyboxProjection::SkyboxProjection()
{

}

SkyboxProjection::~SkyboxProjection()
{
    
}

void SkyboxProjection::LoadImageToSphericalCoords(
        SphereCoordsContainer* coords,
        EnvMapImage* topImage, EnvMapImage* bottomImage, 
        EnvMapImage* leftImage, EnvMapImage* rightImage,
        EnvMapImage* frontImage, EnvMapImage* backImage)
{
    coords->Empty();
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
        
        // We only need to build this matrix once
        Eigen::Matrix3f A = Pmat*D;

        for(int i = 0; i < image->GetWidth(); i++)
        {
            for(int j = 0; j < image->GetHeight(); j++)
            {
                unsigned int pixelData = image->GetPixel(i, j);
                float u = (float)i / (float)(image->GetWidth()-1);
                float v = (float)j / (float)(image->GetHeight()-1);

                // Transform uv to cartesian coordinates
                // Convert f:[u,v,1] -> [x,y,z]
                Eigen::Vector3f X(u, v, 0);
                Eigen::Vector3f Y = f(A*X);

                // Column 3 holds which axis to fit to.
                // To avoid passing another matrix operation, we set it this way.
                for(int k = 0; k < 3; k++)
                    if(Pmat(k, 2) == 1)
                        Y(k) = offset;

                //std::cout << p.z() << "," << std::endl;
                //std::cout << "(" << p.x() << "," << p.y() << "," << p.z() << "), ";
#ifdef STORE_CARTESIAN_COORDS
                coordsCart.push_back(Y);
#endif
                // Lastly, convert to spherical coordinates and store.

                auto newPoint = CartesianToSpherical({
                    Y.x(), Y.y(), Y.z(),
                    pixelData
                });
                coords->AddPoint(newPoint.azimuth, 
                    newPoint.elevation, pixelData);
            }
            //std::cout << std::endl;
        }
    };


    // Go through each image and store the result

    // Start with the top image and bottom images
    {
        // Top-Bottom plane is simple xy plane.
        Eigen::Matrix3f P1;
            P1 << 1, 0, 0,
                0, 1, 0,
                0, 0, 1;
        if(topImage != nullptr)
            uvToCoord(P1, 1, topImage);
        if(bottomImage != nullptr)
            uvToCoord(P1, -1, bottomImage);
    }

    // Do the front and back images
    {
        // Front-Back is the yz plane
        Eigen::Matrix3f P2;
            P2 << 0, 0, 1,
                1, 0, 0,
                0, 1, 0;
        if(backImage != nullptr)
            uvToCoord(P2, 1, backImage);
        if(frontImage != nullptr)
            uvToCoord(P2, -1, frontImage);
    }

    // Do the left and right images
    {
        // Left-Right is the xz plane
        Eigen::Matrix3f P3;
            P3 << 1, 0, 0,
                0, 0, 1,
                0, 1, 0;
        if(rightImage != nullptr)
            uvToCoord(P3, 1, rightImage);
        if(leftImage != nullptr)
            uvToCoord(P3, -1, leftImage);
    }

}

EnvMapImage SkyboxProjection::ConvertToImageTop(SphereCoordsContainer* coords,
    int width, int height)
{
    // Only output the top image
    EnvMapImage newImage(width, height);
    return newImage;
}

std::vector<Eigen::Vector3f> SkyboxProjection::GetCoordsCart()
{
    return coordsCart;
}

// Sign functions
/*
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
template <typename T> float sgnf(T val) {
    return (T(0) < val) - (val < T(0));
}*/

SpherePoint<float> SkyboxProjection::CartesianToSpherical(Point3df point)
{
    //float r = sqrt(point.x * point.x + point.y * point.y + point.z * point.z);
    //float theta = acos(point.z/r);
    float azim = atan2(point.y, point.x);
    float elev = atan2(point.z, sqrt(point.x * point.x + point.y * point.y));
    /*float phi = sgnf(point.y)
        * acos(point.x / sqrt(point.x * point.x + point.y * point.y));
    */
    // Note that we don't save r, because we force this point to be
    // on the unit sphere.
    SpherePoint<float> newPoint;
    newPoint.azimuth = azim;
    newPoint.elevation = elev;
    newPoint.pixelValue = point.pixelValue;
    return newPoint;
}

Point3df SkyboxProjection::SphericalToCartesian(SpherePoint<float> point)
{
    // Remember that r=1 since everything is done against the unit sphere.
    Point3df newPoint;
    newPoint.x = cos(point.elevation) * cos(point.azimuth);
    newPoint.y = cos(point.elevation) * sin(point.azimuth);
    newPoint.z = sin(point.elevation);
    newPoint.pixelValue = point.pixelValue;
    return newPoint;
}