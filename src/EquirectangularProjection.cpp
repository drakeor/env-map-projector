#include "EquirectangularProjection.h"

const double pi = 3.14159265358979323846;

EquirectangularProjection::EquirectangularProjection()
{

}

EquirectangularProjection::~EquirectangularProjection()
{

}

void EquirectangularProjection::LoadImageToSphericalCoords(EnvMapImage* image)
{
    coords.Empty();

    for(int i = 0; i < image->GetWidth(); i++)
    {
        for(int j = 0; j < image->GetHeight(); j++)
        {
            // Convert to UV coords. Domain: [0,1]
            float u = (float)i / (float)(image->GetWidth());
            float v = (float)j / (float)(image->GetHeight());
            unsigned int pixelData = image->GetPixel(i, j);

            // Coordinates are stored internally as spherical coordinates
            // Convert and add to our coordinate collection
            auto sphericalPt = UVToSpherical({u, v, pixelData});
            coords.AddPoint(u, v, pixelData);
        }
    }
}

EnvMapImage EquirectangularProjection::ConvertToImage(int width, int height)
{
    // TODO: We should error check that coords actually has points?

    EnvMapImage newImage(width, height);

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            // Convert to UV coordinates. Domain: [0,1]
            float u = (float)i / (float)(width);
            float v = (float)j / (float)(height);

            // Coordinates are stored as Spherical, so convert and grab the closest point data
            auto sphericalPt = UVToSpherical({u, v, 0});
            auto pointData = coords.GetClosestPoint(sphericalPt.x, sphericalPt.y);
            unsigned int pixelData = pointData.pixelValue;

            // Set pixel in the final UV image
            newImage.SetPixel(u, v, pixelData);
        }
    }

    return newImage;
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

