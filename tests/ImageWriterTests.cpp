#include "../src/Utils/ImageReader.h"
#include "../src/Utils/ImageWriter.h"
#include "../src/Utils/EnvMapImage.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE( "ImageWriter", "[image-writer]" ) {

    SECTION( "bad file" ) {

        ImageReader imageReader;
        auto imageData = imageReader.LoadImage("assets/testimages/good_image.png");

        ImageWriter imageWriter;
        imageWriter.SaveImage("assets/testoutput/imagewriter_test_direct.png", imageData);
    }

    SECTION( "full copy file" ) {

        ImageReader imageReader;
        auto imageData = imageReader.LoadImage("assets/testimages/good_image.png");

        EnvMapImage newImageData(imageData.GetWidth(), imageData.GetHeight());
        for(int i = 0; i < imageData.GetWidth(); i++)
        {
            for(int j = 0; j < imageData.GetHeight(); j++)
            {
                newImageData.SetPixel(i, j, imageData.GetPixel(i, j));
            }
        }
        
        ImageWriter imageWriter;
        imageWriter.SaveImage("assets/testoutput/imagewriter_test_copy.png", newImageData);
    }
}