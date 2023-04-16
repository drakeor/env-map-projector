#ifndef _SKYBOX_PROJECTION_H_
#define _SKYBOX_PROJECTION_H_

#include <tuple>
#include <vector>
#include <array>

#include "../lib/eigen/Eigen/Dense"

#include "../Coordinates/CoordContainerBase.h"
#include "../Utils/EnvMapImage.h"

// Flag to also stores the cartesian coordinates
// Useful for testing and debugging at the expense
// of longer computation and more memory being used
#define STORE_CARTESIAN_COORDS

namespace EnvProj
{
    enum SkyboxSurf : uint32_t
    {
        TopSurf = 0,
        BottomSurf = 1,
        LeftSurf = 2,
        RightSurf = 3,
        FrontSurf = 4,
        BackSurf = 5
    };

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
            uint32_t width, uint32_t height);
            
    private:
        Eigen::Vector3i SideToCoordMap(SkyboxSurf side);
        T SideToConstVal(SkyboxSurf side);
    };

    template class SkyboxProjection<float>;
    template class SkyboxProjection<double>;

}

#endif