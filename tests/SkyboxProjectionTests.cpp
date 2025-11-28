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
        auto topImg = reader.LoadImage("assets/skybox_test_small/top.png");
        auto bottomImg = reader.LoadImage("assets/skybox_test_small/bottom.png");
        auto leftImg = reader.LoadImage("assets/skybox_test_small/left.png");
        auto rightImg = reader.LoadImage("assets/skybox_test_small/right.png");
        auto frontImg = reader.LoadImage("assets/skybox_test_small/front.png");
        auto backImg = reader.LoadImage("assets/skybox_test_small/back.png");

        SkyboxProjection<float> proj;
        std::shared_ptr<CoordContainerBase<float>> coords = 
            proj.LoadImageToSphericalCoords(
                &topImg, &bottomImg,
                &leftImg, &rightImg, &frontImg, &backImg);
    }

    // load test image
    SECTION( "same size test" ) {

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

    SECTION( "hemisphere to skybox sampling" ) {
        const uint32_t hemiSize = 8;
        EnvMapImage topImg(hemiSize, hemiSize);
        EnvMapImage bottomImg(hemiSize, hemiSize);

        auto encodePixel = [](uint32_t surfId, uint32_t x, uint32_t y) {
            return ((surfId & 0xFFu) << 24)
                | ((x & 0xFFu) << 16)
                | ((y & 0xFFu) << 8)
                | 0xFFu;
        };

        for(uint32_t y = 0; y < hemiSize; ++y)
        {
            for(uint32_t x = 0; x < hemiSize; ++x)
            {
                topImg.SetPixel(x, y, encodePixel(1, x, y));
                bottomImg.SetPixel(x, y, encodePixel(2, x, y));
            }
        }

        HemisphericalProjection<float> hemiProj;
        auto coords = hemiProj.LoadImageToSphericalCoords(&topImg, &bottomImg);

        SkyboxProjection<float> skyProj;
        const uint32_t cubeSize = 5;
        auto cubeImgs = skyProj.ConvertToImages(coords.get(), cubeSize);

        auto normalizedCubeDir = [&](SkyboxSurf surf, uint32_t px, uint32_t py) {
            float u = static_cast<float>(px) / static_cast<float>(cubeSize - 1);
            float v = static_cast<float>(py) / static_cast<float>(cubeSize - 1);
            u = 2.0f * u - 1.0f;
            v = 2.0f * v - 1.0f;

            std::array<float, 3> uvCoord = {
                u, v, CoordConversions<float>::SideToConstVal(surf)
            };
            Point3d<int32_t> coordMap = CoordConversions<float>::SideToCoordMap(surf);
            Point3d<float> dir;
            dir.x = uvCoord[coordMap.x];
            dir.y = uvCoord[coordMap.y];
            dir.z = uvCoord[coordMap.z];

            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
            REQUIRE(len > 0.0f);
            dir.x /= len;
            dir.y /= len;
            dir.z /= len;
            return dir;
        };

        auto expectedHemiPixel = [&](const Point3d<float>& dir) {
            const EnvMapImage& hemiImg = (dir.z >= 0.0f) ? topImg : bottomImg;
            auto clampIndex = [&](float value) {
                int idx = static_cast<int>(std::round(value * static_cast<float>(hemiSize)));
                if(idx < 0)
                    idx = 0;
                if(idx >= static_cast<int>(hemiSize))
                    idx = static_cast<int>(hemiSize) - 1;
                return idx;
            };

            float u = (dir.x + 1.0f) * 0.5f;
            float v = (dir.y + 1.0f) * 0.5f;
            int hx = clampIndex(u);
            int hy = clampIndex(v);
            return hemiImg.GetPixel(hx, hy);
        };

        auto assertSample = [&](SkyboxSurf surf, uint32_t px, uint32_t py) {
            Point3d<float> dir = normalizedCubeDir(surf, px, py);
            uint32_t expected = expectedHemiPixel(dir);
            uint32_t actual = cubeImgs[static_cast<size_t>(surf)].GetPixel(px, py);
            REQUIRE(expected == actual);
        };

        // Sample a few directions per face to make sure we're not copying rows/columns.
        assertSample(FrontSurf, cubeSize / 2, cubeSize / 2);
        assertSample(FrontSurf, cubeSize - 1, cubeSize / 2);
        assertSample(RightSurf, cubeSize / 2, cubeSize / 2);
        assertSample(TopSurf, cubeSize / 2, cubeSize / 2);
        assertSample(BottomSurf, cubeSize / 2, cubeSize / 2);
    }
}
