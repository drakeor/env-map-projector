#include "../src/Utils/ImageReader.h"
#include "../src/Utils/ImageWriter.h"
#include "../src/Projections/SkydomeProjection.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace EnvProj;

const float pi = 3.14159265358979323846f;

TEST_CASE( "SkydomeProjection", "[skydome-projection]" ) {

    // load test image
    SECTION( "loading all" ) {

        ImageReader reader;
        auto topImg = reader.LoadImage("assets/skybox_test_small/top.png");
        auto bottomImg = reader.LoadImage("assets/skybox_test_small/bottom.png");

        SkydomeProjection<float> proj;
        /*std::shared_ptr<CoordContainerBase<float>> coords = 
            proj.LoadImageToSphericalCoords(
                &topImg, &bottomImg);*/
    }
}
