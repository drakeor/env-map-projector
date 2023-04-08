#include "../src/SkyboxProjection.h"
#include "../src/ImageReader.h"
#include "../src/ImageWriter.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace std;

TEST_CASE( "SkyboxProjection", "[skybox-projection]" ) {

    // should not cause an error
    SECTION( "initialization" ) {
        SkyboxProjection proj;
    }

    // load test image
    SECTION( "loading" ) {
        SkyboxProjection proj;
        ImageReader reader;
        auto topImg = reader.LoadImage("assets/skybox_test_small/top.png");
        auto bottomImg = reader.LoadImage("assets/skybox_test_small/bottom.png");
        auto leftImg = reader.LoadImage("assets/skybox_test_small/left.png");
        auto rightImg = reader.LoadImage("assets/skybox_test_small/right.png");
        auto frontImg = reader.LoadImage("assets/skybox_test_small/front.png");
        auto backImg = reader.LoadImage("assets/skybox_test_small/back.png");

        proj.LoadImageToSphericalCoords(&topImg, &bottomImg,
            &leftImg, &rightImg, &frontImg, &backImg);
    }

}