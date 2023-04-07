#ifndef _SKYBOX_PROJECTION_H_
#define _SKYBOX_PROJECTION_H_

#include <tuple>

#include "CoordsContainer2d.h"
#include "EnvMapImage.h"
#include "Point2d.h"

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

    void LoadImageToSphericalCoords(EnvMapImage* image);
    EnvMapImage ConvertToImage(int width, int height);

    Point2df UVToSpherical(SkyboxUvPoint uvPoint);
    SkyboxUvPoint SphericalToUV(Point2df uvPoint);

private:
    CoordsContainer2d coords;
};

#endif