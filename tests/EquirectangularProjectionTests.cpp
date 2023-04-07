#include "../src/EquirectangularProjection.h"
#include "../src/ImageReader.h"
#include "../src/ImageWriter.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace std;

TEST_CASE( "EquirectangularProjection", "[equirectangular-projection]" ) {

    // should not cause an error
    SECTION( "initialization" ) {
        EquirectangularProjection proj;
    }

    // load test image
    SECTION( "loading" ) {
        EquirectangularProjection proj;
        ImageReader reader;
        auto img = reader.LoadImage("assets/testimages/pixel_equirectangle_test.png");
        proj.LoadImageToSphericalCoords(&img);
    }

    // UV To Spherical and back should be reversable.
    SECTION( "uv to spherical reciprocity" ) {

        const float epsilon = 1e-6f;
        EquirectangularProjection proj;

        Point2df uvPoint;
        uvPoint.x = 0.175f;
        uvPoint.y = 0.834f;
        uvPoint.pixelValue = 0xAABBCCDD;

        Point2df spherePoint = proj.UVToSpherical(uvPoint);
        Point2df reformedUvPoint = proj.SphericalToUV(spherePoint);

        REQUIRE( abs(uvPoint.x - reformedUvPoint.x) <  epsilon);
        REQUIRE( abs(uvPoint.y - reformedUvPoint.y) <  epsilon);
        REQUIRE( uvPoint.pixelValue == reformedUvPoint.pixelValue );

    }

    // load test image
    SECTION( "samesize" ) {
        EquirectangularProjection proj;
        ImageReader reader;
        auto img = reader.LoadImage("assets/testimages/pixel_equirectangle_test.png");
        proj.LoadImageToSphericalCoords(&img);
        
        auto newImg = proj.ConvertToImage(200, 100);

        ImageWriter writer;
        writer.SaveImage("assets/testoutput/test_samesize_equirect.png", newImg);
    }

    // load test image
    SECTION( "upsize" ) {
        EquirectangularProjection proj;
        ImageReader reader;
        auto img = reader.LoadImage("assets/testimages/pixel_equirectangle_test.png");
        proj.LoadImageToSphericalCoords(&img);
        
        auto newImg = proj.ConvertToImage(500, 220);

        ImageWriter writer;
        writer.SaveImage("assets/testoutput/test_upsize_equirect.png", newImg);
    }

    // load test image
    SECTION( "downsize" ) {
        EquirectangularProjection proj;
        ImageReader reader;
        auto img = reader.LoadImage("assets/testimages/pixel_equirectangle_test.png");
        proj.LoadImageToSphericalCoords(&img);
        
        auto newImg = proj.ConvertToImage(30, 20);

        ImageWriter writer;
        writer.SaveImage("assets/testoutput/test_downsize_equirect.png", newImg);
    }
}