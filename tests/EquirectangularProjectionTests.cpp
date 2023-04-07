#include "../src/EquirectangularProjection.h"
#include "../src/ImageReader.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

TEST_CASE( "EquirectangularProjection", "[equirectangular-projection]" ) {

    // should not cause an error
    SECTION( "initialization" ) {
        EquirectangularProjection proj;
    }

    // load test image
    SECTION( "initialization" ) {
        EquirectangularProjection proj;
        ImageReader reader;
        auto img = reader.LoadImage("assets/testimages/pixel_equirectangle_test.png");
        proj.LoadImageToSphericalCoords(&img);
    }
}