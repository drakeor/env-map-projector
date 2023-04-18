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
    SECTION( "direct detect" ) {

        CoordContainerHemiSpherical<float> coords(5);

        coords.SetPointDirect(TopHemiSurf, 0, 0, 0xAA);
        REQUIRE( coords.GetClosestPixel(-1, -1, 1) == 0xAA );

        coords.SetPointDirect(BottomHemiSurf, 0, 0, 0xBB);
        REQUIRE( coords.GetClosestPixel(-1, -1, -1) == 0xBB );

        coords.SetPointDirect(TopHemiSurf, 4, 4, 0xCC);
        REQUIRE( coords.GetClosestPixel(1, 1, 1) == 0xCC );

        coords.SetPointDirect(BottomHemiSurf, 4, 4, 0xDD);
        REQUIRE( coords.GetClosestPixel(1, 1, -1) == 0xDD);

    }

    // Indirectly hitting a point should work
    SECTION( "indirect detect" ) {

        CoordContainerHemiSpherical<float> coords(5);

        coords.SetPointDirect(TopHemiSurf, 2, 2, 0xCC);
        REQUIRE( coords.GetClosestPixel(-0.25f, -0.25f, 0.75f) == 0xCC );

        coords.SetPointDirect(BottomHemiSurf, 3, 3, 0xDD);
        REQUIRE( coords.GetClosestPixel(0.25f, 0.25f, -0.75f) == 0xDD);

    }

    SECTION( "spherical to cartesian detect" ) {

        CoordContainerHemiSpherical<float> coords(5);

        coords.SetPointDirect(TopHemiSurf, 4, 3, 0xEE);
        REQUIRE( coords.GetClosestPixel(0.1f,0) == 0xEE);

        coords.SetPointDirect(BottomHemiSurf, 3, 3, 0xFF);
        REQUIRE( coords.GetClosestPixel(0.1f,-pi/2+0.01f) == 0xFF);

    }

}
