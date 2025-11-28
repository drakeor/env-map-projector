#include "../src/Utils/ImageReader.h"
#include "../src/Utils/ImageWriter.h"
#include "../src/Projections/SkyboxProjection.h"
#include "../src/Projections/HemisphericalProjection.h"
#include "../src/Coordinates/CoordConversions.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <cmath>
#include <array>

using namespace EnvProj;

const float pi = 3.14159265358979323846f;

TEST_CASE( "SkyboxProjection", "[skybox-projection]" ) {

    // load test image
    SECTION( "loading all" ) {

        ImageReader reader;
        auto topImg = reader.LoadImage("assets/testimages/skybox_test_small/top.png");
        auto bottomImg = reader.LoadImage("assets/testimages/skybox_test_small/bottom.png");
        auto leftImg = reader.LoadImage("assets/testimages/skybox_test_small/left.png");
        auto rightImg = reader.LoadImage("assets/testimages/skybox_test_small/right.png");
        auto frontImg = reader.LoadImage("assets/testimages/skybox_test_small/front.png");
        auto backImg = reader.LoadImage("assets/testimages/skybox_test_small/back.png");

        SkyboxProjection<float> proj;
        std::shared_ptr<CoordContainerBase<float>> coords = 
            proj.LoadImageToSphericalCoords(
                &topImg, &bottomImg,
                &leftImg, &rightImg, &frontImg, &backImg);
    }

    // load test image
    SECTION( "same size test" ) {

        ImageReader reader;
        auto topImg = reader.LoadImage("assets/testimages/skybox_test_small/top.png");
        auto bottomImg = reader.LoadImage("assets/testimages/skybox_test_small/bottom.png");
        auto leftImg = reader.LoadImage("assets/testimages/skybox_test_small/left.png");
        auto rightImg = reader.LoadImage("assets/testimages/skybox_test_small/right.png");
        auto frontImg = reader.LoadImage("assets/testimages/skybox_test_small/front.png");
        auto backImg = reader.LoadImage("assets/testimages/skybox_test_small/back.png");
        SkyboxProjection<double> proj;
        std::shared_ptr<CoordContainerBase<double>> coords = 
            proj.LoadImageToSphericalCoords(
                &topImg, &bottomImg,
                &leftImg, &rightImg, &frontImg, &backImg);

        
        std::array<EnvMapImage, 6> newImgs = proj.ConvertToImages(
            coords.get(), 5);

        ImageWriter writer;
        writer.SaveImage("assets/testoutput/skybox_same_size_top.png", newImgs[0]);
        writer.SaveImage("assets/testoutput/skybox_same_size_bottom.png", newImgs[1]);
        writer.SaveImage("assets/testoutput/skybox_same_size_left.png", newImgs[2]);
        writer.SaveImage("assets/testoutput/skybox_same_size_right.png", newImgs[3]);
        writer.SaveImage("assets/testoutput/skybox_same_size_front.png", newImgs[4]);
        writer.SaveImage("assets/testoutput/skybox_same_size_back.png", newImgs[5]);
    }

    // load test image
    SECTION( "upsize test" ) {

        ImageReader reader;
        auto topImg = reader.LoadImage("assets/testimages/skybox_test_small/top.png");
        auto bottomImg = reader.LoadImage("assets/testimages/skybox_test_small/bottom.png");
        auto leftImg = reader.LoadImage("assets/testimages/skybox_test_small/left.png");
        auto rightImg = reader.LoadImage("assets/testimages/skybox_test_small/right.png");
        auto frontImg = reader.LoadImage("assets/testimages/skybox_test_small/front.png");
        auto backImg = reader.LoadImage("assets/testimages/skybox_test_small/back.png");
        SkyboxProjection<double> proj;
        std::shared_ptr<CoordContainerBase<double>> coords = 
            proj.LoadImageToSphericalCoords(
                &topImg, &bottomImg,
                &leftImg, &rightImg, &frontImg, &backImg);

        
        std::array<EnvMapImage, 6> newImgs = proj.ConvertToImages(
            coords.get(), 10);

        ImageWriter writer;
        writer.SaveImage("assets/testoutput/skybox_upsize_top.png", newImgs[0]);
        writer.SaveImage("assets/testoutput/skybox_upsize_bottom.png", newImgs[1]);
        writer.SaveImage("assets/testoutput/skybox_upsize_left.png", newImgs[2]);
        writer.SaveImage("assets/testoutput/skybox_upsize_right.png", newImgs[3]);
        writer.SaveImage("assets/testoutput/skybox_upsize_front.png", newImgs[4]);
        writer.SaveImage("assets/testoutput/skybox_upsize_back.png", newImgs[5]);
    }

}
