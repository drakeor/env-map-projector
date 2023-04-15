#include "../src/Utils/ImageReader.h"
#include "../src/Coordinates/CoordContainerSpherical.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace EnvProj;

const float pi = 3.14159265358979323846f;

TEST_CASE( "CoordContainerSpherical", "[coords-container-spherical]" ) {

    // Things should be zero'd by default
    SECTION( "zero detect" ) {

        CoordContainerSpherical<float> coords(4, 4);
        REQUIRE( coords.GetClosestPixel(pi*1.7f, pi*1.7f) == 0x00 );
        REQUIRE( coords.GetClosestPixel(0, 0) == 0x00 );
    }

    // Directly hitting a point should work
    SECTION( "direct detect" ) {

        CoordContainerSpherical<float> coords(4, 4);

        coords.SetPoint(0, 0, 0xAA);
        coords.SetPoint(0, pi, 0xBB);
        coords.SetPoint(pi, 0, 0xCC);
        coords.SetPoint(pi, pi, 0xDD);

        REQUIRE( coords.GetClosestPixel(0, 0) == 0xAA );
        REQUIRE( coords.GetClosestPixel(0, pi) == 0xBB );
        REQUIRE( coords.GetClosestPixel(pi, 0) == 0xCC );
        REQUIRE( coords.GetClosestPixel(pi, pi) == 0xDD );
    }

    // Hitting close to a point should work
    SECTION( "near detect" ) {

        CoordContainerSpherical<float> coords(4, 4);

        coords.SetPoint(0, 0, 0xAA);
        coords.SetPoint(0, pi, 0xBB);
        coords.SetPoint(pi, 0, 0xCC);
        coords.SetPoint(pi, pi, 0xDD);

        REQUIRE( coords.GetClosestPixel(0, pi*1.1f) == 0xBB );
        REQUIRE( coords.GetClosestPixel(0.2f, 0.2f ) == 0xAA );
    }

    // Hitting far away to a point that it's populated
    // should return 0 since that point isn't set
    SECTION( "far detect" ) {

        CoordContainerSpherical<float> coords(4, 4);

        coords.SetPoint(0, 0, 0xAA);
        coords.SetPoint(0, pi, 0xBB);
        coords.SetPoint(pi, 0, 0xCC);

        REQUIRE( coords.GetClosestPixel(pi*1.7f, pi*1.7f) == 0x00 );
    }

    // Cartesian coordinate detection
    SECTION( "cart detect" ) {

        CoordContainerSpherical<float> coords(4, 4);

        coords.SetPoint(0, 0, 0xAA);
        coords.SetPoint(0, pi, 0xBB);
        coords.SetPoint(pi, 0, 0xCC);
        coords.SetPoint(pi, pi, 0xDD);

        // (1,0,0) is closest to 0xAA
        REQUIRE( coords.GetClosestPixel(1, 0, 0) == 0xAA );
        // Opposite side should be closest to 0xCC (since it's pi)
        REQUIRE( coords.GetClosestPixel(-1, 0, 0) == 0xCC );
    }

}
