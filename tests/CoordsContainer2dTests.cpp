#include "../src/CoordsContainer2d.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

TEST_CASE( "CoordsContainer2d", "[coords-container-2d]" ) {

    SECTION( "initialization" ) {
        CoordsContainer2d coordsContainer;
    }

    SECTION( "add points" ) {
        CoordsContainer2d coordsContainer;
        coordsContainer.AddPoint(1, 2, 0xFF00FF00);
        coordsContainer.AddPoint(2, 2, 0xFF00FF00);
    }

    SECTION( "get closest point case 1" ) {
        CoordsContainer2d coordsContainer;
        coordsContainer.AddPoint(-1, -1, 0xFFAABBCC);
        coordsContainer.AddPoint(1, 2, 0xFFAABBCC);
        coordsContainer.AddPoint(2, 3, 0xFFCCDDCC);
        coordsContainer.AddPoint(5, 5, 0xFFBBAADD);

        auto point = coordsContainer.GetClosestPoint(1,1);
        REQUIRE( point.x == 1);
        REQUIRE( point.y == 2);
        REQUIRE( point.pixelValue == 0xFFAABBCC);

    }

    SECTION( "get closest point case 2" ) {
        CoordsContainer2d coordsContainer;
        coordsContainer.AddPoint(-1, -1, 0xFFAABBCC);
        coordsContainer.AddPoint(1, 2, 0xFFAABBCC);
        coordsContainer.AddPoint(200, 300, 0xFFCCDDCC);
        coordsContainer.AddPoint(5, 5, 0xFFBBAADD);

        auto point = coordsContainer.GetClosestPoint(1000,1000);
        REQUIRE( point.x == 200);
        REQUIRE( point.y == 300);
        REQUIRE( point.pixelValue == 0xFFCCDDCC);

    }

}