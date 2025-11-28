#include "Projections/SkyboxProjection.h"
#include "Projections/EquirectangularProjection.h"
#include "Projections/HemisphericalProjection.h"
#include "Utils/ImageReader.h"
#include "Utils/ImageWriter.h"

using namespace EnvProj;

int main()
{
    ImageReader reader;
    auto srcImg = reader.LoadImage("assets/testimages/equirectangular_image.jpg");

    EquirectangularProjection<double> sourceProj;
    auto coords = sourceProj.LoadImageToSphericalCoords(&srcImg);
    ImageWriter writer;

    SkyboxProjection<double> destProj;
    std::array<EnvMapImage, 6> destImgs = destProj.ConvertToImages(coords.get(), 2048);
    
    writer.SaveImage("assets/testoutput/huge_converted_skybox_top.png", destImgs[0]);
    writer.SaveImage("assets/testoutput/huge_converted_skybox_bottom.png", destImgs[1]);
    writer.SaveImage("assets/testoutput/huge_converted_skybox_left.png", destImgs[2]);
    writer.SaveImage("assets/testoutput/huge_converted_skybox_right.png", destImgs[3]);
    writer.SaveImage("assets/testoutput/huge_converted_skybox_front.png", destImgs[4]);
    writer.SaveImage("assets/testoutput/huge_converted_skybox_back.png", destImgs[5]);

    HemisphericalProjection<double> destProj2;
    std::array<EnvMapImage, 2> destImgs2 = destProj2.ConvertToImages(coords.get(), 2048);
    
    writer.SaveImage("assets/testoutput/huge_converted_hemi_top.png", destImgs2[0]);
    writer.SaveImage("assets/testoutput/huge_converted_hemi_bottom.png", destImgs2[1]);

    return 0;
}
