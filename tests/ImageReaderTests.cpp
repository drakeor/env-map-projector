#include "../src/Utils/ImageReader.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE( "ImageReader", "[image-reader]" ) {

    SECTION( "bad file" ) {

        ImageReader imageReader;
        auto imageData = imageReader.LoadImage("missingimage.jpg");

        REQUIRE( !imageData.IsValid() );
    }

    SECTION( "corrupt file" ) {

        ImageReader imageReader;
        auto imageData = imageReader.LoadImage("assets/testimages/corrupt_image.png");

        REQUIRE( !imageData.IsValid() );
    }


    SECTION( "good file" ) {

        ImageReader imageReader;
        auto imageData = imageReader.LoadImage("assets/testimages/good_image.png");

        REQUIRE( imageData.IsValid() );
        REQUIRE( imageData.GetWidth() == 500);
        REQUIRE( imageData.GetHeight() == 500);
        REQUIRE( imageData.GetDataSize() == (500 * 500 * 4));
    }

}