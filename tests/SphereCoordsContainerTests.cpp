#include "../src/SphereCoordsContainer.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

TEST_CASE( "CoordsContainer2d", "[coords-container-2d]" ) {

    // KNN should initialize without erroring
    SECTION( "initialization" ) {
        SphereCoordsContainer coordsContainer(1);
    }

    // Adding points function should work
    SECTION( "add points" ) {
        SphereCoordsContainer coordsContainer(2);
        coordsContainer.AddPoint({1, 2, 0xFF00FF00});
        coordsContainer.AddPoint({2, 2, 0xFF00FF00});
    }

    // Testing direct case of grabbing the closest point
    SECTION( "get closest point direct case" ) {
        SphereCoordsContainer coordsContainer(4);
        coordsContainer.AddPoint({-1, -1, 0xFFAABBCC});
        coordsContainer.AddPoint({1, 2, 0xFFAABBCC});
        coordsContainer.AddPoint({2, 3, 0xFFCCDDCC});
        coordsContainer.AddPoint({5, 5, 0xFFBBAADD});

        auto point = coordsContainer.GetClosestPoint(1,1);
        REQUIRE( point.azimuth == 1);
        REQUIRE( point.elevation == 2);
        REQUIRE( point.pixelValue == 0xFFAABBCC);

    }

    // Testing the indirect case of getting only one point
    // even if it's exactly between two other points
    // We expect the latest one added to result in the closest
    SECTION( "get closest point equidistance case" ) {
        SphereCoordsContainer coordsContainer(3);
        coordsContainer.AddPoint({-1, -1, 0xFFAABBCC});
        coordsContainer.AddPoint({1, 1, 0xFFBBAADD});
        coordsContainer.AddPoint({3, 3, 0xFFBBAADD});

        // I assume it picks the most recent point
        auto point = coordsContainer.GetClosestPoint(2,2);
        REQUIRE( point.azimuth == 3);
        REQUIRE( point.elevation == 3);
        REQUIRE( point.pixelValue == 0xFFBBAADD);

    }

    // Testing grabbing super short cases in the KNN tree
    SECTION( "get closest point extremely short case" ) {
        SphereCoordsContainer coordsContainer(3);
        coordsContainer.AddPoint({-1, -1, 0xFFAABBCC});
        coordsContainer.AddPoint({10.001f, 10.001f, 0xFFAABBCC});
        coordsContainer.AddPoint({30, 30, 0xFFBBAADD});

        auto point = coordsContainer.GetClosestPoint(20,20);
        REQUIRE( point.azimuth == 10.001f);
        REQUIRE( point.elevation == 10.001f);
        REQUIRE( point.pixelValue == 0xFFAABBCC);

    }

    // Testing grabbing faraway points in the KNN tree
    // make sure distance and stuff doesn't overflow
    SECTION( "get closest point long case" ) {
        SphereCoordsContainer coordsContainer(4);
        coordsContainer.AddPoint({-1, -1, 0xFFAABBCC});
        coordsContainer.AddPoint({1, 2, 0xFFAABBCC});
        coordsContainer.AddPoint({200, 300, 0xFFCCDDCC});
        coordsContainer.AddPoint({5, 5, 0xFFBBAADD});

        auto point = coordsContainer.GetClosestPoint(1000,1000);
        REQUIRE( point.azimuth == 200);
        REQUIRE( point.elevation == 300);
        REQUIRE( point.pixelValue == 0xFFCCDDCC);

    }

}