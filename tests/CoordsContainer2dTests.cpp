#include "../src/CoordsContainer2d.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

TEST_CASE( "CoordsContainer2d", "[coords-container-2d]" ) {

    // KNN should initialize without erroring
    SECTION( "initialization" ) {
        CoordsContainer2d coordsContainer;
    }

    // Adding points function should work
    SECTION( "add points" ) {
        CoordsContainer2d coordsContainer;
        coordsContainer.AddPoint(1, 2, 0xFF00FF00);
        coordsContainer.AddPoint(2, 2, 0xFF00FF00);
    }

    // Clearing points function should work
    SECTION( "clear points" ) {
        CoordsContainer2d coordsContainer;
        coordsContainer.Empty();
    }


    // Clearing points and adding only one point should result
    // in that one point being returned regardless.
    SECTION( "clear points and add more" ) {
        CoordsContainer2d coordsContainer;
        coordsContainer.AddPoint(1, 1, 0xFF00FF00);
        coordsContainer.AddPoint(2, 2, 0xFF00FF00);

        coordsContainer.Empty();

        coordsContainer.AddPoint(3, 3, 0xFF00FF00);

        auto point = coordsContainer.GetClosestPoint(1,1);
        REQUIRE( point.x == 3);
        REQUIRE( point.y == 3);
    }

    // Testing direct case of grabbing the closest point
    SECTION( "get closest point direct case" ) {
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

    // Testing the indirect case of getting only one point
    // even if it's exactly between two other points
    // We expect the latest one added to result in the closest
    SECTION( "get closest point equidistance case" ) {
        CoordsContainer2d coordsContainer;
        coordsContainer.AddPoint(-1, -1, 0xFFAABBCC);
        coordsContainer.AddPoint(1, 1, 0xFFBBAADD);
        coordsContainer.AddPoint(3, 3, 0xFFBBAADD);

        // I assume it picks the most recent point
        auto point = coordsContainer.GetClosestPoint(2,2);
        REQUIRE( point.x == 3);
        REQUIRE( point.y == 3);
        REQUIRE( point.pixelValue == 0xFFBBAADD);

    }

    // Testing grabbing super short cases in the KNN tree
    SECTION( "get closest point extremely short case" ) {
        CoordsContainer2d coordsContainer;
        coordsContainer.AddPoint(-1, -1, 0xFFAABBCC);
        coordsContainer.AddPoint(10.001f, 10.001f, 0xFFAABBCC);
        coordsContainer.AddPoint(30, 30, 0xFFBBAADD);

        auto point = coordsContainer.GetClosestPoint(20,20);
        REQUIRE( point.x == 10.001f);
        REQUIRE( point.y == 10.001f);
        REQUIRE( point.pixelValue == 0xFFAABBCC);

    }

    // Testing grabbing faraway points in the KNN tree
    // make sure distance and stuff doesn't overflow
    SECTION( "get closest point long case" ) {
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