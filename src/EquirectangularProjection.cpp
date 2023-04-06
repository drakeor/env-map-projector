#include "EquirectangularProjection.h"

const double pi = 3.14159265358979323846;

void EquirectangularProjection::LoadImageToSphericalCoords(EnvMapImage* image)
{
    coords.Empty();

    for(int i = 0; i < image->GetWidth(); i++)
    {
        for(int j = 0; j < image->GetHeight(); j++)
        {
            float u = (float)i / (float)(image->GetWidth());
            float v = (float)j / (float)(image->GetHeight());
            unsigned int pixelData = image->GetPixel(i, j);

            auto sphericalPt = UVToSpherical({u, v, pixelData});
            coords.AddPoint(u, v, pixelData);
        }
    }
}

Point2df EquirectangularProjection::UVToSpherical(Point2df inputPt)
{
    float theta = inputPt.x * 2 * pi;
    inputPt.x = theta;

    float phi = inputPt.y * pi;
    inputPt.y = phi;
    
    return inputPt;
}

Point2df EquirectangularProjection::SphericalToUV(Point2df inputPt)
{
    float u = inputPt.x / (2 * pi);
    inputPt.x = u;

    float v = inputPt.y / pi;
    inputPt.y = v;
    
    return inputPt;
}

