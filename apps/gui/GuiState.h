#ifndef GUI_STATE_H
#define GUI_STATE_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "Utils/EnvMapImage.h"

enum class ProjectionType
{
    Equirectangular = 0,
    Skybox = 1,
    Hemispherical = 2
};

enum class AutoScaleMode
{
    None = 0,
    ScaleToLargest,
    ScaleToSmallest
};

struct GuiState
{
    ProjectionType inputProjection = ProjectionType::Equirectangular;
    ProjectionType outputProjection = ProjectionType::Skybox;
    AutoScaleMode autoScaleMode = AutoScaleMode::ScaleToLargest;
    float outputScale = 1.0f;

    int uiScaleIndex = 1;
    float uiScale = 2.0f;

    int outputEquirectWidth = 0;
    int outputEquirectHeight = 0;
    int outputSkyboxSize = 0;
    int outputHemisphericalSize = 0;

    std::array<char, 1024> outputDirectory = {"output"};
    std::array<char, 256> outputPrefix = {"converted"};

    bool conversionInProgress = false;
    float conversionProgress = 0.0f;
    std::string statusText;

    std::vector<EnvMapImage> convertedImages;
    std::vector<std::string> convertedLabels;
    uint64_t convertedGeneration = 0;
};

#endif
