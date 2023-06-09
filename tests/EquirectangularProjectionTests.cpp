#include "../src/Utils/ImageReader.h"
#include "../src/Utils/ImageWriter.h"
#include "../src/Projections/EquirectangularProjection.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace EnvProj;

const float pi = 3.14159265358979323846f;

TEST_CASE( "EquirectangularProjection", "[equirectangular-projection]" ) {

    // Loading test images
    SECTION( "loading test image" ) {

        ImageReader reader;
        auto img = reader.LoadImage("assets/testimages/pixel_equirectangle_test.png");
        
        EquirectangularProjection<float> proj;
        std::shared_ptr<CoordContainerBase<float>> coords
            = proj.LoadImageToSphericalCoords(&img);
    }

    SECTION( "samesize" ) {

        ImageReader reader;
        auto img = reader.LoadImage("assets/testimages/pixel_equirectangle_test.png");
        
        EquirectangularProjection<float> proj;
        std::shared_ptr<CoordContainerBase<float>> coords
            = proj.LoadImageToSphericalCoords(&img);

        auto newImg = proj.ConvertToImage(coords.get(), img.GetWidth(), img.GetHeight());
        
        ImageWriter writer;
        writer.SaveImage("assets/testoutput/test_samesize_equirect.png", newImg);
    }
 
    SECTION( "downsize" ) {

        ImageReader reader;
        auto img = reader.LoadImage("assets/testimages/pixel_equirectangle_test.png");
        
        EquirectangularProjection<float> proj;
        std::shared_ptr<CoordContainerBase<float>> coords
            = proj.LoadImageToSphericalCoords(&img);

        auto newImg = proj.ConvertToImage(coords.get(), img.GetWidth()/2, img.GetHeight()/2);
        
        ImageWriter writer;
        writer.SaveImage("assets/testoutput/test_downsize_equirect.png", newImg);
    }

    SECTION( "upsize" ) {

        ImageReader reader;
        auto img = reader.LoadImage("assets/testimages/pixel_equirectangle_test.png");
        
        EquirectangularProjection<float> proj;
        std::shared_ptr<CoordContainerBase<float>> coords
            = proj.LoadImageToSphericalCoords(&img);

        auto newImg = proj.ConvertToImage(coords.get(), img.GetWidth()*2, img.GetHeight()*2);
        
        ImageWriter writer;
        writer.SaveImage("assets/testoutput/test_upsize_equirect.png", newImg);
    }
}
