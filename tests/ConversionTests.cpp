#include "../src/Utils/ImageReader.h"
#include "../src/Utils/ImageWriter.h"
#include "../src/Projections/SkyboxProjection.h"
#include "../src/Projections/SkydomeProjection.h"
#include "../src/Projections/EquirectangularProjection.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace EnvProj;

const float pi = 3.14159265358979323846f;

TEST_CASE( "ConversionTests", "[conversion]" ) {

// load test image
    SECTION( "skybox to equirectangular 1" ) {

        ImageReader reader;
        auto topImg = reader.LoadImage("assets/skybox_test_small/top.png");
        auto bottomImg = reader.LoadImage("assets/skybox_test_small/bottom.png");
        auto leftImg = reader.LoadImage("assets/skybox_test_small/left.png");
        auto rightImg = reader.LoadImage("assets/skybox_test_small/right.png");
        auto frontImg = reader.LoadImage("assets/skybox_test_small/front.png");
        auto backImg = reader.LoadImage("assets/skybox_test_small/back.png");

        SkyboxProjection<double> proj;
        std::shared_ptr<CoordContainerBase<double>> coords = 
            proj.LoadImageToSphericalCoords(
                &topImg, &bottomImg,
                &leftImg, &rightImg, &frontImg, &backImg);

        EquirectangularProjection<double> rectProj;
        auto newImg = rectProj.ConvertToImage(coords.get(),
            800, 400);

        ImageWriter writer;
        writer.SaveImage("assets/testoutput/skybox_to_equirect_1.png", newImg);
    }

    // load test image
    SECTION( "skybox to equirectangular 2" ) {

        ImageReader reader;
        auto topImg = reader.LoadImage("assets/skybox/top.jpg");
        auto bottomImg = reader.LoadImage("assets/skybox/bottom.jpg");
        auto leftImg = reader.LoadImage("assets/skybox/left.jpg");
        auto rightImg = reader.LoadImage("assets/skybox/right.jpg");
        auto frontImg = reader.LoadImage("assets/skybox/front.jpg");
        auto backImg = reader.LoadImage("assets/skybox/back.jpg");

        SkyboxProjection<double> proj;
        std::shared_ptr<CoordContainerBase<double>> coords = 
            proj.LoadImageToSphericalCoords(
                &topImg, &bottomImg,
                &leftImg, &rightImg, &frontImg, &backImg);

        EquirectangularProjection<double> rectProj;
        auto newImg = rectProj.ConvertToImage(coords.get(),
            800, 400);

        ImageWriter writer;
        writer.SaveImage("assets/testoutput/skybox_to_equirect_2.png", newImg);
    }

    SECTION( "skybox to skydome" ) {

        ImageReader reader;
        auto topImg = reader.LoadImage("assets/skybox/top.jpg");
        auto bottomImg = reader.LoadImage("assets/skybox/bottom.jpg");
        auto leftImg = reader.LoadImage("assets/skybox/left.jpg");
        auto rightImg = reader.LoadImage("assets/skybox/right.jpg");
        auto frontImg = reader.LoadImage("assets/skybox/front.jpg");
        auto backImg = reader.LoadImage("assets/skybox/back.jpg");

        SkyboxProjection<double> proj;
        std::shared_ptr<CoordContainerBase<double>> coords = 
            proj.LoadImageToSphericalCoords(
                &topImg, &bottomImg,
                &leftImg, &rightImg, &frontImg, &backImg);

        SkydomeProjection<double> destProj;
        std::array<EnvMapImage, 2> newImgs = destProj.ConvertToImages(
            coords.get(), 512);

        ImageWriter writer;
        writer.SaveImage("assets/testoutput/skybox_to_skydome_top.png", newImgs[0]);
        writer.SaveImage("assets/testoutput/skybox_to_skydome_bottom.png", newImgs[1]);
    }

    SECTION( "equirectangular to skybox" ) {

        ImageReader reader;
        auto testImg = reader.LoadImage("assets/testimages/pixel_equirectangle_test.png");

        EquirectangularProjection<double> sourceProj;
        std::shared_ptr<CoordContainerBase<double>> coords = 
            sourceProj.LoadImageToSphericalCoords(&testImg);

        SkyboxProjection<double> destProj;
        std::array<EnvMapImage, 6> newImgs = destProj.ConvertToImages(
            coords.get(), 50);

        ImageWriter writer;
        writer.SaveImage("assets/testoutput/equirect_to_skybox_top.png", newImgs[0]);
        writer.SaveImage("assets/testoutput/equirect_to_skybox_bottom.png", newImgs[1]);
        writer.SaveImage("assets/testoutput/equirect_to_skybox_left.png", newImgs[2]);
        writer.SaveImage("assets/testoutput/equirect_to_skybox_right.png", newImgs[3]);
        writer.SaveImage("assets/testoutput/equirect_to_skybox_front.png", newImgs[4]);
        writer.SaveImage("assets/testoutput/equirect_to_skybox_back.png", newImgs[5]);
    }
}
