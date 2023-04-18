#ifndef _HEMISPHERICAL_PROJECTION_H_
#define _HEMISPHERICAL_PROJECTION_H_

#include <iostream>
#include <vector>
#include <memory>
#include <array>

#include "../../lib/eigen/Eigen/Dense"

#include "../Coordinates/CoordContainerBase.h"
#include "../Utils/EnvMapImage.h"
#include "../Coordinates/PointSphere.h"

namespace EnvProj
{
    template <typename T>
    class HemisphericalProjection
    {
    public:
        HemisphericalProjection();
        ~HemisphericalProjection();

        std::shared_ptr<CoordContainerBase<T>> 
            LoadImageToSphericalCoords(EnvMapImage* topImage, EnvMapImage* bottomImage);
        std::array<EnvMapImage,2> ConvertToImages(CoordContainerBase<T>* coords,
            uint32_t imageSideLength);
        //PointSphere<T> UVToSpherical(T u, T v);

    private:
    };

    template class HemisphericalProjection<float>;
    template class HemisphericalProjection<double>;
}

#endif