#include "../src/Utils/ImageReader.h"
#include "../src/Utils/ImageWriter.h"
#include "../src/Projections/SkydomeProjection.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace EnvProj;

const float pi = 3.14159265358979323846f;

TEST_CASE( "SkydomeProjection", "[skydome-projection]" ) {

    // load test image
    SECTION( "loading top" ) {

        ImageReader reader;
        auto topImg = reader.LoadImage("assets/testimages/skydome_gradient.png");

        SkydomeProjection<float> proj;
        std::shared_ptr<CoordContainerBase<float>> coords = 
            proj.LoadImageToSphericalCoords(
                &topImg, nullptr);

        std::array<EnvMapImage, 2> newImgs = proj.ConvertToImages(
            coords.get(), 256);
            
        ImageWriter writer;
        writer.SaveImage("assets/testoutput/skydome_top_test.png", newImgs[0]);
        writer.SaveImage("assets/testoutput/skydome_bottom_test.png", newImgs[1]);
    }
}
