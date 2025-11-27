#include "ProjectionSizing.h"

#include <algorithm>
#include <cmath>

namespace ProjectionSizing
{
namespace
{
constexpr int kMinDimension = 16;
constexpr double kPi = 3.14159265358979323846;
constexpr double kSqrtPi = 1.7724538509055159;
int ClampDimension(int value)
{
    return std::max(value, kMinDimension);
}

int RoundToInt(double value)
{
    return static_cast<int>(std::lround(value));
}
}

SizeSet FromEquirectangular(int width, int height, HemisphereSizingMode mode)
{
    SizeSet sizes{};
    int w = ClampDimension(width);
    int h = ClampDimension(height);
    sizes.equirectWidth = w;
    sizes.equirectHeight = h;

    int faceFromWidth = w / 4;
    int faceFromHeight = h / 2;
    int face = 0;
    if(faceFromWidth > 0 && faceFromHeight > 0)
    {
        face = (faceFromWidth + faceFromHeight) / 2;
    }
    else if(faceFromWidth > 0)
    {
        face = faceFromWidth;
    }
    else if(faceFromHeight > 0)
    {
        face = faceFromHeight;
    }
    else
    {
        face = kMinDimension;
    }

    sizes.cubemapFace = ClampDimension(face);
    if(mode == HemisphereSizingMode::ArtifactReduction)
    {
        int hemi = RoundToInt(static_cast<double>(w) / kSqrtPi);
        sizes.hemisphereSize = ClampDimension(hemi);
    }
    else
    {
        sizes.hemisphereSize = h;
    }
    return sizes;
}

SizeSet FromCubemapFace(int faceSize, HemisphereSizingMode mode)
{
    SizeSet sizes{};
    int face = ClampDimension(faceSize);
    sizes.cubemapFace = face;
    sizes.equirectWidth = ClampDimension(face * 4);
    sizes.equirectHeight = ClampDimension(face * 2);
    if(mode == HemisphereSizingMode::ArtifactReduction)
    {
        int hemi = RoundToInt((4.0 * face) / kSqrtPi);
        sizes.hemisphereSize = ClampDimension(hemi);
    }
    else
    {
        sizes.hemisphereSize = ClampDimension(face * 2);
    }
    return sizes;
}

SizeSet FromHemispherical(int squareSize, HemisphereSizingMode mode)
{
    SizeSet sizes{};
    int square = ClampDimension(squareSize);
    sizes.hemisphereSize = square;
    if(mode == HemisphereSizingMode::ArtifactReduction)
    {
        double hemi = static_cast<double>(square);
        int width = RoundToInt(kSqrtPi * hemi);
        sizes.equirectWidth = ClampDimension(width);
        sizes.equirectHeight = ClampDimension(RoundToInt(static_cast<double>(width) * 0.5));
        int face = RoundToInt(static_cast<double>(width) * 0.25);
        sizes.cubemapFace = ClampDimension(face);
    }
    else
    {
        sizes.equirectWidth = ClampDimension(square * 2);
        sizes.equirectHeight = square;
        int face = square / 2;
        if(face <= 0)
            face = kMinDimension;
        sizes.cubemapFace = ClampDimension(face);
    }
    return sizes;
}

}
