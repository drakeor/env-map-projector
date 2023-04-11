#include "CoordsContainer2d.h"
#include "SkyboxProjection.h"
#include "ImageReader.h"

int main()
{
    CoordsContainer2d coords;
    SkyboxProjection proj;
    ImageReader reader;

    auto topImg = reader.LoadImage("assets/skybox_test_small/top.png");
    auto bottomImg = reader.LoadImage("assets/skybox_test_small/bottom.png");
    auto leftImg = reader.LoadImage("assets/skybox_test_small/left.png");
    auto rightImg = reader.LoadImage("assets/skybox_test_small/right.png");
    auto frontImg = reader.LoadImage("assets/skybox_test_small/front.png");
    auto backImg = reader.LoadImage("assets/skybox_test_small/back.png");

    SkyboxProjection skyboxProj;
    skyboxProj.LoadImageToSphericalCoords(
        &coords, 
        &topImg, &bottomImg,
        &leftImg, &rightImg, &frontImg, &backImg);

    auto sphericalPoints = coords.GetAllPoints();
    std::cout << "theta = [";
    for(int i = 0; i < sphericalPoints.size(); i++)
    {
        std::cout << sphericalPoints[i].x << ",";
    }
    std:: cout << "]" << std::endl << std::endl;

    std::cout << "phi = [";
    for(int i = 0; i < sphericalPoints.size(); i++)
    {
        std::cout << sphericalPoints[i].y << ",";
    }
    std:: cout << "]" << std::endl << std::endl;
    
    return 0;
}