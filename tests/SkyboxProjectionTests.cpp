#include "../src/Utils/ImageReader.h"
#include "../src/Projections/SkyboxProjection.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace EnvProj;

const float pi = 3.14159265358979323846f;

TEST_CASE( "SkyboxProjection", "[skybox-projection]" ) {

    // Things should be zero'd by default
    SECTION( "zero detect" ) {

        SkyboxProjection<float> proj;
        std::shared_ptr<CoordContainerBase<float>> coords
            = proj.LoadImageToSphericalCoords()
    }

}
