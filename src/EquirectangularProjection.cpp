#include "EquirectangularProjection.h"

const float pi = 3.14159265358979323846f;

EquirectangularProjection::EquirectangularProjection()
{

}

EquirectangularProjection::~EquirectangularProjection()
{

}

void EquirectangularProjection::LoadImageToSphericalCoords(
    SphereCoordsContainer* coords, EnvMapImage* image)
{
    coords->Empty();

    for(int i = 0; i < image->GetWidth(); i++)
    {
        for(int j = 0; j < image->GetHeight(); j++)
        {
            // Convert to UV coords. Domain: [0,1]
            float u = (float)i / (float)(image->GetWidth()-1);
            float v = (float)j / (float)(image->GetHeight()-1);
            unsigned int pixelData = image->GetPixel(i, j);

            // Coordinates are stored internally as spherical coordinates
            // Convert and add to our coordinate collection
            auto sphericalPt = UVToSpherical({u, v, pixelData});
            coords->AddPoint(sphericalPt.azimuth, sphericalPt.elevation,
                pixelData);
        }
    }
}

EnvMapImage EquirectangularProjection::ConvertToImage(SphereCoordsContainer* coords, 
    int width, int height)
{
    // TODO: We should error check that coords actually has points?

    EnvMapImage newImage(width, height);

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            // Convert to UV coordinates. Domain: [0,1]
            float u = (float)i / (float)(width-1);
            float v = (float)j / (float)(height-1);

            // Coordinates are stored as Spherical, so convert and grab the closest point data
            Point2df uvPoint;
            uvPoint.x = u;
            uvPoint.y = v;

            auto sphericalPt = UVToSpherical(uvPoint);
            auto pointData = coords->GetClosestPoint(sphericalPt.azimuth,
                sphericalPt.elevation);
            unsigned int pixelData = pointData.pixelValue;

            // Set pixel in the final UV image
            newImage.SetPixel(i, j, pixelData);
        }
    }

    return newImage;
}


SpherePointf EquirectangularProjection::UVToSpherical(Point2df inputPt)
{
    SpherePointf newPoint;
    newPoint.azimuth = (inputPt.x * 2.0f * pi) - pi;
    newPoint.elevation = (inputPt.y * pi) - (pi/2);
    newPoint.pixelValue = inputPt.pixelValue;

    return newPoint;
}

Point2df EquirectangularProjection::SphericalToUV(SpherePointf inputPt)
{
    Point2df newPoint;
    newPoint.x = (inputPt.azimuth + pi) / (2.0f * pi);
    newPoint.y = (inputPt.elevation + (pi/2))/ pi;
    newPoint.pixelValue = inputPt.pixelValue;

    return newPoint;
}

