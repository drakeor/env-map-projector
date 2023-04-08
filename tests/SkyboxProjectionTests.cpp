#include "../src/ImageReader.h"
#include "../src/ImageWriter.h"
#include "../src/SkyboxProjection.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <cmath>

// Define this so we can verify the cartesian coordinates

using namespace std;

constexpr float eps = 1e-3f;

TEST_CASE( "SkyboxProjection", "[skybox-projection]" ) {

    // should not cause an error
    SECTION( "initialization" ) {
        SkyboxProjection proj;
    }

    // load test image
    SECTION( "loading all" ) {
        SkyboxProjection proj;
        ImageReader reader;
        auto topImg = reader.LoadImage("assets/skybox_test_small/top.png");
        auto bottomImg = reader.LoadImage("assets/skybox_test_small/bottom.png");
        auto leftImg = reader.LoadImage("assets/skybox_test_small/left.png");
        auto rightImg = reader.LoadImage("assets/skybox_test_small/right.png");
        auto frontImg = reader.LoadImage("assets/skybox_test_small/front.png");
        auto backImg = reader.LoadImage("assets/skybox_test_small/back.png");

        proj.LoadImageToSphericalCoords(&topImg, &bottomImg,
            &leftImg, &rightImg, &frontImg, &backImg);
    }

    // coordinate conversion to spherical
    SECTION( "cartesian to spherical conversion" ) {
        SkyboxProjection proj;

        Point3df point;
        point.x = 1;
        point.y = 1;
        point.z = 1;
        point.pixelValue = 0xAABBCCDD;

        auto newPoint = proj.CartesianToSpherical(point);
        
        REQUIRE( newPoint.pixelValue == point.pixelValue );
        REQUIRE( (newPoint.x - 0.9553f) <  eps);
        REQUIRE( (newPoint.y - 0.7854f) <  eps);
    }

    // coordinate conversion to cartesian
    SECTION( "spherical to cartesian conversion" ) {
        SkyboxProjection proj;

        Point2df point;
        point.x = 0.9553f;
        point.y = 0.7854f;
        point.pixelValue = 0xAABBCCDD;

        auto newPoint = proj.SphericalToCartesian(point);
        
        REQUIRE( newPoint.pixelValue == point.pixelValue );
        REQUIRE( (newPoint.x - 1.0f) <  eps);
        REQUIRE( (newPoint.y - 1.0f) <  eps);
        REQUIRE( (newPoint.z - 1.0f) <  eps);
    }

    // make sure top coordinates line up as expecting
    SECTION( "top cartesian coordinates testing" ) {
        SkyboxProjection proj;
        ImageReader reader;

        auto img = reader.LoadImage("assets/skybox_test_small/top.png");
        proj.LoadImageToSphericalCoords(&img, nullptr,
            nullptr, nullptr, nullptr, nullptr);

        auto vect = proj.GetCoordsCart();
        REQUIRE( vect.size() == 25);
        REQUIRE( vect[0].z() == 1 );
        REQUIRE( vect[6].z() == 1 );
        REQUIRE( vect[11].z() == 1 );

        REQUIRE( vect[0].x() == -1.0f );
        REQUIRE( vect[0].y() == -1.0f );

        REQUIRE( vect[24].x() == 1.0f );
        REQUIRE( vect[24].y() == 1.0f );
    }

    // make sure bottom coordinates line up as expecting
    SECTION( "bottom cartesian coordinates testing" ) {
        SkyboxProjection proj;
        ImageReader reader;

        auto img = reader.LoadImage("assets/skybox_test_small/bottom.png");
        proj.LoadImageToSphericalCoords(nullptr, &img,
            nullptr, nullptr, nullptr, nullptr);

        auto vect = proj.GetCoordsCart();
        REQUIRE( vect.size() == 25);
        REQUIRE( vect[0].z() == -1 );
        REQUIRE( vect[6].z() == -1 );
        REQUIRE( vect[11].z() == -1 );

        REQUIRE( vect[0].x() == -1.0f );
        REQUIRE( vect[0].y() == -1.0f );

        REQUIRE( vect[24].x() == 1.0f );
        REQUIRE( vect[24].y() == 1.0f );
    }

    // make sure left coordinates line up as expecting
    SECTION( "left cartesian coordinates testing" ) {
        SkyboxProjection proj;
        ImageReader reader;

        auto img = reader.LoadImage("assets/skybox_test_small/left.png");
        proj.LoadImageToSphericalCoords(nullptr, nullptr,
            &img, nullptr, nullptr, nullptr);

        auto vect = proj.GetCoordsCart();
        REQUIRE( vect.size() == 25);
        REQUIRE( vect[0].y() == -1 );
        REQUIRE( vect[6].y() == -1 );
        REQUIRE( vect[11].y() == -1 );

        REQUIRE( vect[0].x() == -1.0f );
        REQUIRE( vect[0].z() == -1.0f );

        REQUIRE( vect[24].x() == 1.0f );
        REQUIRE( vect[24].z() == 1.0f );
    }

    // make sure right coordinates line up as expecting
    SECTION( "right cartesian coordinates testing" ) {
        SkyboxProjection proj;
        ImageReader reader;

        auto img = reader.LoadImage("assets/skybox_test_small/right.png");
        proj.LoadImageToSphericalCoords(nullptr, nullptr,
            nullptr, &img, nullptr, nullptr);

        auto vect = proj.GetCoordsCart();
        REQUIRE( vect.size() == 25);
        REQUIRE( vect[0].y() == 1 );
        REQUIRE( vect[6].y() == 1 );
        REQUIRE( vect[11].y() == 1 );

        REQUIRE( vect[0].x() == -1.0f );
        REQUIRE( vect[0].z() == -1.0f );

        REQUIRE( vect[24].x() == 1.0f );
        REQUIRE( vect[24].z() == 1.0f );
    }

    // make sure front coordinates line up as expecting
    SECTION( "front cartesian coordinates testing" ) {
        SkyboxProjection proj;
        ImageReader reader;

        auto img = reader.LoadImage("assets/skybox_test_small/front.png");
        proj.LoadImageToSphericalCoords(nullptr, nullptr,
            nullptr, nullptr, &img, nullptr);

        auto vect = proj.GetCoordsCart();
        REQUIRE( vect.size() == 25);
        REQUIRE( vect[0].x() == -1 );
        REQUIRE( vect[6].x() == -1 );
        REQUIRE( vect[11].x() == -1 );

        REQUIRE( vect[0].y() == -1.0f );
        REQUIRE( vect[0].z() == -1.0f );

        REQUIRE( vect[24].y() == 1.0f );
        REQUIRE( vect[24].z() == 1.0f );
    }

    // make sure back coordinates line up as expecting
    SECTION( "back cartesian coordinates testing" ) {
        SkyboxProjection proj;
        ImageReader reader;

        auto img = reader.LoadImage("assets/skybox_test_small/back.png");
        proj.LoadImageToSphericalCoords(nullptr, nullptr,
            nullptr, nullptr, nullptr, &img);

        auto vect = proj.GetCoordsCart();
        REQUIRE( vect.size() == 25);
        REQUIRE( vect[0].x() == 1 );
        REQUIRE( vect[6].x() == 1 );
        REQUIRE( vect[11].x() == 1 );

        REQUIRE( vect[0].y() == -1.0f );
        REQUIRE( vect[0].z() == -1.0f );

        REQUIRE( vect[24].y() == 1.0f );
        REQUIRE( vect[24].z() == 1.0f );
    }
}