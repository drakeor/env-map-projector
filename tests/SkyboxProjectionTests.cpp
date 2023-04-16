#include "../src/Utils/ImageReader.h"
#include "../src/Projections/SkyboxProjection.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace EnvProj;

const float pi = 3.14159265358979323846f;

TEST_CASE( "SkyboxProjection", "[skybox-projection]" ) {

    // load test image
    SECTION( "loading all" ) {

        ImageReader reader;
        auto topImg = reader.LoadImage("assets/skybox_test_small/top.png");
        auto bottomImg = reader.LoadImage("assets/skybox_test_small/bottom.png");
        auto leftImg = reader.LoadImage("assets/skybox_test_small/left.png");
        auto rightImg = reader.LoadImage("assets/skybox_test_small/right.png");
        auto frontImg = reader.LoadImage("assets/skybox_test_small/front.png");
        auto backImg = reader.LoadImage("assets/skybox_test_small/back.png");

        SkyboxProjection<float> proj;
        std::shared_ptr<CoordContainerBase<float>> coords = 
            proj.LoadImageToSphericalCoords(
                &topImg, &bottomImg,
                &leftImg, &rightImg, &frontImg, &backImg);
    }
}
