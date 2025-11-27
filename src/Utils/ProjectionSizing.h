#ifndef PROJECTION_SIZING_H
#define PROJECTION_SIZING_H

namespace ProjectionSizing
{
enum class HemisphereSizingMode
{
    Identity = 0,
    ArtifactReduction
};

struct SizeSet
{
    int equirectWidth;
    int equirectHeight;
    int cubemapFace;
    int hemisphereSize;
};

SizeSet FromEquirectangular(int width, int height, HemisphereSizingMode mode = HemisphereSizingMode::Identity);
SizeSet FromCubemapFace(int faceSize, HemisphereSizingMode mode = HemisphereSizingMode::Identity);
SizeSet FromHemispherical(int squareSize, HemisphereSizingMode mode = HemisphereSizingMode::Identity);
}

#endif
