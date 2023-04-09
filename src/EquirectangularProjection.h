#ifndef _EQUIRECTANGULAR_PROJECTION_H_
#define _EQUIRECTANGULAR_PROJECTION_H_

#include <iostream>
#include <vector>
#include "CoordsContainer2d.h"
#include "EnvMapImage.h"
#include "Point2d.h"

class EquirectangularProjection
{
public:
    EquirectangularProjection();
    ~EquirectangularProjection();

    void LoadImageToSphericalCoords(CoordsContainer2d* coords, 
        EnvMapImage* image);
    EnvMapImage ConvertToImage(CoordsContainer2d* coords,
        int width, int height);

    Point2df UVToSpherical(Point2df uvPoint);
    Point2df SphericalToUV(Point2df uvPoint);

private:
    CoordsContainer2d coords;
};

#endif