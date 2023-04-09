#ifndef _SKYBOX_PROJECTION_H_
#define _SKYBOX_PROJECTION_H_

#include <tuple>
#include <vector>

#include "../lib/eigen/Eigen/Dense"

#include "CoordsContainer2d.h"
#include "EnvMapImage.h"
#include "Point2d.h"
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

    void LoadImageToSphericalCoords(
        CoordsContainer2d* coords,
        EnvMapImage* topImage, EnvMapImage* bottomImage, 
        EnvMapImage* leftImage, EnvMapImage* rightImage,
        EnvMapImage* frontImage, EnvMapImage* backImage);
        
    EnvMapImage ConvertToImageTop(CoordsContainer2d* coords,
        int width, int height);
        
    std::vector<Eigen::Vector3f> GetCoordsCart();

    Point2df CartesianToSpherical(Point3df point);
    Point3df SphericalToCartesian(Point2df spherical);

private:
    std::vector<Eigen::Vector3f> coordsCart;
};

#endif