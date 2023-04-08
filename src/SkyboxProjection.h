#ifndef _SKYBOX_PROJECTION_H_
#define _SKYBOX_PROJECTION_H_

#include <tuple>
#include <vector>

#include "../lib/eigen/Eigen/Dense"

#include "CoordsContainer2d.h"
#include "EnvMapImage.h"
#include "Point2d.h"

// Flag to also stores the cartesian coordinates
// Useful for testing and debugging at the expense
// of longer computation and more memory being used
#define STORE_CARTESIAN_COORDS

enum SkyboxFace
{
    Top,
    Bottom,
    Front,
    Back,
    Left,
    Right
};

using SkyboxUvPoint = std::tuple<Point2df, SkyboxFace>;

class SkyboxProjection
{
public:
    SkyboxProjection();
    ~SkyboxProjection();

    void LoadImageToSphericalCoords(
        EnvMapImage* topImage, EnvMapImage* bottomImage, 
        EnvMapImage* leftImage, EnvMapImage* rightImage,
        EnvMapImage* frontImage, EnvMapImage* backImage);
        
    EnvMapImage ConvertToImageTop(int width, int height);

private:
    CoordsContainer2d coords;
#ifdef STORE_CARTESIAN_COORDS
    std::vector<Eigen::Vector3f> coordsCart;
#endif
};

#endif