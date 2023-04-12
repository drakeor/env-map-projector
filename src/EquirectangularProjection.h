#ifndef _EQUIRECTANGULAR_PROJECTION_H_
#define _EQUIRECTANGULAR_PROJECTION_H_

#include <iostream>
#include <vector>
#include <memory>

#include "SphereCoordsContainer.h"
#include "EnvMapImage.h"
#include "Point2d.h"
#include "SpherePoint.h"

class EquirectangularProjection
{
public:
    EquirectangularProjection();
    ~EquirectangularProjection();

    std::shared_ptr<SphereCoordsContainer> LoadImageToSphericalCoords(
        EnvMapImage* image);
    EnvMapImage ConvertToImage(SphereCoordsContainer* coords,
        int width, int height);

    SpherePointf UVToSpherical(Point2df uvPoint);
    Point2df SphericalToUV(SpherePointf uvPoint);

private:
};

#endif