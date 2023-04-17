#include "../src/Utils/ImageReader.h"
#include "../src/Coordinates/CoordContainerHemiSpherical.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace EnvProj;

const float pi = 3.14159265358979323846f;

TEST_CASE( "CoordContainerHemiSpherical", "[coords-container-hemi-spherical]" ) {

    // Things should be zero'd by default
    SECTION( "zero detect" ) {

        CoordContainerHemiSpherical<float> coords(4);
        REQUIRE( coords.GetClosestPixel(pi*1.7f, pi*1.7f) == 0x00 );
        REQUIRE( coords.GetClosestPixel(0, 0) == 0x00 );
    }

    // Directly hitting a point should work
    SECTION( "direct point set" ) {

        CoordContainerHemiSpherical<float> coords(5);

        coords.SetPointDirect(TopHemiSurf, 2, 2, 0xAA);
        REQUIRE( coords.GetClosestPixel(0, 0) == 0xAA );
    }

    // Directly hitting a point should work
    SECTION( "indirect detect" ) {

        CoordContainerHemiSpherical<float> coords(5);

        coords.SetPoint(0, 0, 0xAA);
        REQUIRE( coords.GetClosestPixel(0, 0) == 0xAA );

        coords.SetPoint(0, -1.0f, 0xBB);
        REQUIRE( coords.GetClosestPixel(0, -1.0f) == 0xBB );

        coords.SetPoint(0, 1.5f, 0xCC);
        REQUIRE( coords.GetClosestPixel(0, 1.5f) == 0xCC );

        coords.SetPoint(3.0f, 1.5f, 0xDD);
        REQUIRE( coords.GetClosestPixel(3.0f, 1.5f) == 0xDD );

        coords.SetPoint(-1.5f, -1.5f, 0xEE);
        REQUIRE( coords.GetClosestPixel(-1.5f, -1.5f) == 0xEE );
    }

}
