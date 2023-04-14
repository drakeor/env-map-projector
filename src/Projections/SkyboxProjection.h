#ifndef _SKYBOX_PROJECTION_H_
#define _SKYBOX_PROJECTION_H_

#include <tuple>
#include <vector>

#include "../lib/eigen/Eigen/Dense"

#include "SphereCoordsContainer.h"
#include "EnvMapImage.h"
#include "SpherePoint.h"
#include "Point3d.h"

// Flag to also stores the cartesian coordinates
// Useful for testing and debugging at the expense
// of longer computation and more memory being used
#define STORE_CARTESIAN_COORDS

class SkyboxProjection
{
public:
    SkyboxProjection();
    ~SkyboxProjection();

    std::shared_ptr<SphereCoordsContainer> LoadImageToSphericalCoords(
        EnvMapImage* topImage, EnvMapImage* bottomImage, 
        EnvMapImage* leftImage, EnvMapImage* rightImage,
        EnvMapImage* frontImage, EnvMapImage* backImage);
        
    EnvMapImage ConvertToImageTop(SphereCoordsContainer* coords,
        int width, int height);
        
    std::vector<Eigen::Vector3f> GetCoordsCart();

    SpherePoint<float> CartesianToSpherical(Point3df point);
    Point3df SphericalToCartesian(SpherePoint<float> spherical);

private:
    std::vector<Eigen::Vector3f> coordsCart;
};

#endif