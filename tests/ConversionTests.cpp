#include "../src/ImageReader.h"
#include "../src/ImageWriter.h"
#include "../src/SphereCoordsContainer.h"
#include "../src/EquirectangularProjection.h"
#include "../src/SkyboxProjection.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

TEST_CASE( "ConversionTests", "[conversion-tests]" ) {

    // KNN should initialize without erroring
    SECTION( "skybox to equirectangular" ) {
        ImageReader reader;

        auto topImg = reader.LoadImage("assets/skybox_test_basic/top.png");
        auto bottomImg = reader.LoadImage("assets/skybox_test_basic/bottom.png");
        auto leftImg = reader.LoadImage("assets/skybox_test_basic/left.png");
        auto rightImg = reader.LoadImage("assets/skybox_test_basic/right.png");
        auto frontImg = reader.LoadImage("assets/skybox_test_basic/front.png");
        auto backImg = reader.LoadImage("assets/skybox_test_basic/back.png");

        SkyboxProjection skyboxProj;
        auto coords = skyboxProj.LoadImageToSphericalCoords(
            &topImg, &bottomImg,
            &leftImg, &rightImg, &frontImg, &backImg);

        EquirectangularProjection equiRectangularProj;
        auto newImg = equiRectangularProj.ConvertToImage(coords.get(),
            400, 400);

        ImageWriter writer;
        writer.SaveImage("assets/testoutput/skybox_to_equirect.png", newImg);

    }


}