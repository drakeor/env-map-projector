#include "../src/ImageReader.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

TEST_CASE( "EnvMapImage", "[env-map-image]" ) {

    SECTION( "pixel checking bad input" ) {

        ImageReader imageReader;
        EnvMapImage imageData = imageReader.LoadImage("assets/testimages/pixel_map_test.png");


        REQUIRE( imageData.IsValid() );
        REQUIRE( imageData.GetWidth() == 100);
        REQUIRE( imageData.GetHeight() == 100);

        REQUIRE( imageData.GetPixel(-1, -1) == 0x00000000);
        REQUIRE( imageData.GetPixel(0, 100) == 0x00000000);
        REQUIRE( imageData.GetPixel(100, 0) == 0x00000000);
        REQUIRE( imageData.GetPixel(100, 100) == 0x00000000);
    }

    SECTION( "pixel checking" ) {

        ImageReader imageReader;
        EnvMapImage imageData = imageReader.LoadImage("assets/testimages/pixel_map_test.png");

        /*std::cout << std::endl;
        for(int i = 0; i < imageData.GetWidth(); i++)
        {
            for(int j = 0; j < imageData.GetHeight(); j++)
            {
                std::cout << std::hex << imageData.GetPixel(j,i) << " ";
            }
            std::cout << std::endl;
        }*/

        REQUIRE( imageData.IsValid() );
        REQUIRE( imageData.GetWidth() == 100);
        REQUIRE( imageData.GetHeight() == 100);

        // Red channel tests
        REQUIRE( imageData.GetPixel(0,0) == 0xFF0000FF);
        REQUIRE( imageData.GetPixel(1,0) == 0xFF0000FF);
        REQUIRE( imageData.GetPixel(2,0) == 0xFF0000FF);
        REQUIRE( imageData.GetPixel(1,10) == 0xFF0000FF);
        REQUIRE( imageData.GetPixel(2,10) == 0xFF0000FF);

        // Blue channel tests
        REQUIRE( imageData.GetPixel(0,25) == 0x0000FFFF);
        REQUIRE( imageData.GetPixel(1,25) == 0x0000FFFF);
        REQUIRE( imageData.GetPixel(48,75) == 0x0000FFFF);
        REQUIRE( imageData.GetPixel(49,75) == 0x0000FFFF);

        // Green channel tests
        REQUIRE( imageData.GetPixel(51,25) == 0x00FF00FF);
        REQUIRE( imageData.GetPixel(52,25) == 0x00FF00FF);
        REQUIRE( imageData.GetPixel(51,75) == 0x00FF00FF);
        REQUIRE( imageData.GetPixel(52,75) == 0x00FF00FF);

        // White channel tests
        REQUIRE( imageData.GetPixel(0,76) == 0xFFFFFFFF);
        REQUIRE( imageData.GetPixel(1,76) == 0xFFFFFFFF);
        REQUIRE( imageData.GetPixel(98,99) == 0xFFFFFFFF);
        REQUIRE( imageData.GetPixel(99,99) == 0xFFFFFFFF);
    }
}