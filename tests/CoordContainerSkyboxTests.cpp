#include "../src/Utils/ImageReader.h"
#include "../src/Coordinates/CoordContainerSkybox.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace EnvProj;

const float pi = 3.14159265358979323846f;

TEST_CASE( "CoordContainerSkybox", "[coords-container-skybox]" ) {

    // Things should be zero'd by default
    SECTION( "zero detect" ) {

        CoordContainerSkybox<float> coords(10);
        REQUIRE( coords.GetClosestPixel(0, 0) == 0 );
    }

    // Directly hitting a point should work
    SECTION( "direct detect" ) {

        CoordContainerSkybox<float> coords(10);

        coords.SetPoint(1, 0, 0, 0x000000AA);
        REQUIRE( coords.GetClosestPixel(1, 0, 0) == 0x000000AA );

        coords.SetPoint(0, 1, 0, 0x000000BB);
        REQUIRE( coords.GetClosestPixel(0, 1, 0) == 0x000000BB );
        
        coords.SetPoint(0, 0, 1, 0x000000CC);
        REQUIRE( coords.GetClosestPixel(0, 0, 1) == 0x000000CC );
    }

    // Hitting near a point should work
    SECTION( "direct detect" ) {

        CoordContainerSkybox<float> coords(10);

        coords.SetPoint(1, 0.5f, 0.5f, 0x000000AA);
        REQUIRE( coords.GetClosestPixel(1, 0.55f, 0.55f) == 0x000000AA );

        coords.SetPoint(0.5f, 1, 0.5f, 0x000000CC);
        REQUIRE( coords.GetClosestPixel(0.52f, 1, 0.48f) == 0x000000CC );
    }

    // Directly hitting a point should work
    SECTION( "cartesian detect" ) {

        CoordContainerSkybox<float> coords(10);

        coords.SetPoint(1, 0, 0, 0x000000AA);
        REQUIRE( coords.GetClosestPixel(1, 0, 0) == 0x000000AA );

        coords.SetPoint(0, 1, 0, 0x000000BB);
        REQUIRE( coords.GetClosestPixel(0, 1, 0) == 0x000000BB );
        
        coords.SetPoint(0, 0, 1, 0x000000CC);
        REQUIRE( coords.GetClosestPixel(0, 0, 1) == 0x000000CC );
    }

    // Converting to spherical should work fine
    SECTION( "cartesian detect" ) {

        CoordContainerSkybox<float> coords(80);

        coords.SetPoint(1, 0, 0, 0x000000AA);
        coords.SetPoint(0, 1, 0, 0x000000BB);
        coords.SetPoint(0, 0, 1, 0x000000CC);
        coords.SetPoint(-1, -1, 1, 0x000000DD);

        REQUIRE( coords.GetClosestPixel(0, 0) == 0x000000AA );
        //REQUIRE( coords.GetClosestPixel(0, 1.5708f) == 0x000000BB );
        REQUIRE( coords.GetClosestPixel(1.6f, 0) == 0x000000BB );
        REQUIRE( coords.GetClosestPixel(0, 1.5708f) == 0x000000CC );
        REQUIRE( coords.GetClosestPixel(-2.3562f, 0.6155f) == 0x000000DD );

    }

}
