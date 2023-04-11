#include "SphereCoordsContainer.h"
#include "SkyboxProjection.h"
#include "EquirectangularProjection.h"
#include "ImageReader.h"
#include "ImageWriter.h"

int main()
{
    SphereCoordsContainer coords;
    SkyboxProjection proj;
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

    auto sphericalPoints = coords.GetAllPoints();

    EquirectangularProjection equiProj;
    auto img = equiProj.ConvertToImage(&coords, 500, 250);

    ImageWriter writer;
    writer.SaveImage("converted_output.png", img);

    /*
    std::vector<Point3df> cartCoords;
    for(int i = 0; i < sphericalPoints.size(); i++)
    {
        Point3df newPoint = proj.SphericalToCartesian(sphericalPoints[i]);
        cartCoords.push_back(newPoint);
    }
    */

/*
    std::cout << "x = [";
    for(int i = 0; i < cartCoords.size(); i++)
    {
        std::cout << cartCoords[i].x << ",";
    }
    std:: cout << "]" << std::endl << std::endl;

    std::cout << "y = [";
    for(int i = 0; i < cartCoords.size(); i++)
    {
        std::cout << cartCoords[i].y << ",";
    }
    std:: cout << "]" << std::endl << std::endl;

    std::cout << "z = [";
    for(int i = 0; i < cartCoords.size(); i++)
    {
        std::cout << cartCoords[i].z << ",";
    }
    std:: cout << "]" << std::endl << std::endl;
*/

    /*
    std::cout << "azim = [";
    for(int i = 0; i < sphericalPoints.size(); i++)
    {
        std::cout << sphericalPoints[i].azimuth << ",";
    }
    std:: cout << "]" << std::endl << std::endl;

    std::cout << "evel = [";
    for(int i = 0; i < sphericalPoints.size(); i++)
    {
        std::cout << sphericalPoints[i].elevation << ",";
    }
    std:: cout << "]" << std::endl << std::endl;
    */


    
    return 0;
}