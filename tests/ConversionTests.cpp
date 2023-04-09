#include "../src/ImageReader.h"
#include "../src/ImageWriter.h"
#include "../src/CoordsContainer2d.h"
#include "../src/EquirectangularProjection.h"
#include "../src/SkyboxProjection.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

TEST_CASE( "ConversionTests", "[conversion-tests]" ) {

    // KNN should initialize without erroring
    SECTION( "skybox to equirectangular" ) {
        CoordsContainer2d coords;
        ImageReader reader;

        auto topImg = reader.LoadImage("assets/skybox_test_basic/top.png");
        auto bottomImg = reader.LoadImage("assets/skybox_test_basic/bottom.png");
        auto leftImg = reader.LoadImage("assets/skybox_test_basic/left.png");
        auto rightImg = reader.LoadImage("assets/skybox_test_basic/right.png");
        auto frontImg = reader.LoadImage("assets/skybox_test_basic/front.png");
        auto backImg = reader.LoadImage("assets/skybox_test_basic/back.png");

        SkyboxProjection skyboxProj;
        skyboxProj.LoadImageToSphericalCoords(
            &coords, 
            &topImg, &bottomImg,
            &leftImg, &rightImg, &frontImg, &backImg);

        EquirectangularProjection equiRectangularProj;
        auto newImg = equiRectangularProj.ConvertToImage(&coords,
            400, 200);

        ImageWriter writer;
        writer.SaveImage("assets/testoutput/skybox_to_equirect.png", newImg);

    }


}