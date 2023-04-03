#include "../src/CoordsContainer2d.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

TEST_CASE( "CoordsContainer2d", "[coords-container-2d]" ) {

    SECTION( "initialization" ) {
        CoordsContainer2d coordsContainer;
    }

    SECTION( "add points" ) {
        CoordsContainer2d coordsContainer;
        coordsContainer.AddPoint(1, 2, 12345);
        coordsContainer.AddPoint(2, 2, 12345);
    }

    SECTION( "get closest point" ) {
        CoordsContainer2d coordsContainer;
        coordsContainer.AddPoint(1, 2, 12345);
        coordsContainer.AddPoint(2, 2, 12345);
    }

}