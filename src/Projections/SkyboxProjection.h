#ifndef _SKYBOX_PROJECTION_H_
#define _SKYBOX_PROJECTION_H_

#include <tuple>
#include <vector>
#include <array>

#include <Eigen/Dense>

#include "../Coordinates/CoordContainerBase.h"
#include "../Utils/EnvMapImage.h"
#include "../Coordinates/SkyboxSurf.h"

namespace EnvProj
{
    template<typename T>
    class SkyboxProjection
    {
    public:
        SkyboxProjection();
        ~SkyboxProjection();

        std::shared_ptr<CoordContainerBase<T>> LoadImageToSphericalCoords(
            EnvMapImage* topImage, EnvMapImage* bottomImage, 
            EnvMapImage* leftImage, EnvMapImage* rightImage,
            EnvMapImage* frontImage, EnvMapImage* backImage);
            
        std::array<EnvMapImage, 6> ConvertToImages(CoordContainerBase<T>* coords,
            uint32_t sideLength);
            
    private:
    };

    template class SkyboxProjection<float>;
    template class SkyboxProjection<double>;
}

#endif