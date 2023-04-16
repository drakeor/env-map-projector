#include "Projections/SkyboxProjection.h"
#include "Projections/EquirectangularProjection.h"
#include "Utils/ImageReader.h"
#include "Utils/ImageWriter.h"

using namespace EnvProj;

int main()
{
    
    SkyboxProjection<double> skyboxProj;
    ImageReader reader;

    auto topImg = reader.LoadImage("assets/skybox_big/top.jpg");
    auto bottomImg = reader.LoadImage("assets/skybox_big/bottom.jpg");
    auto leftImg = reader.LoadImage("assets/skybox_big/left.jpg");
    auto rightImg = reader.LoadImage("assets/skybox_big/right.jpg");
    auto frontImg = reader.LoadImage("assets/skybox_big/front.jpg");
    auto backImg = reader.LoadImage("assets/skybox_big/back.jpg");

    auto coords = skyboxProj.LoadImageToSphericalCoords(
        &topImg, &bottomImg,
        &leftImg, &rightImg, &frontImg, &backImg);


    EquirectangularProjection<double> equiProj;
    auto img = equiProj.ConvertToImage(coords.get(), 2048, 1024);

    ImageWriter writer;
    writer.SaveImage("converted_output.png", img);
    

/*
    std::vector<Point3df> cartCoords;
    for(int i = 0; i < sphericalPoints.size(); i++)
    {
        Point3df newPoint = proj.SphericalToCartesian(sphericalPoints[i]);
        cartCoords.push_back(newPoint);
    }
    
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