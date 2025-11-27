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
    AutoScaleMode autoScaleMode = AutoScaleMode::None;

    int outputEquirectWidth = 2048;
    int outputEquirectHeight = 1024;
    int outputCubeSide = 1024;

    std::array<char, 512> outputDirectory = {"assets/testoutput"};
    std::array<char, 128> outputPrefix = {"converted"};

    bool conversionInProgress = false;
    std::string statusText;

    std::vector<EnvMapImage> convertedImages;
    std::vector<std::string> convertedLabels;
    uint64_t convertedGeneration = 0;
};

#endif
