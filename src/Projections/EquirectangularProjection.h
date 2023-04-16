#ifndef _EQUIRECTANGULAR_PROJECTION_H_
#define _EQUIRECTANGULAR_PROJECTION_H_

#include <iostream>
#include <vector>
#include <memory>

#include "../../lib/eigen/Eigen/Dense"

#include "../Coordinates/CoordContainerBase.h"
#include "../Utils/EnvMapImage.h"
#include "../Coordinates/PointSphere.h"

namespace EnvProj
{
    template <typename T>
    class EquirectangularProjection
    {
    public:
        EquirectangularProjection();
        ~EquirectangularProjection();

        std::shared_ptr<CoordContainerBase<T>> 
            LoadImageToSphericalCoords(EnvMapImage* image);
        EnvMapImage ConvertToImage(CoordContainerBase<T>* coords,
            int width, int height);

        PointSphere<T> UVToSpherical(T u, T v);

    private:
    };

    template class EquirectangularProjection<float>;
    template class EquirectangularProjection<double>;
}

#endif