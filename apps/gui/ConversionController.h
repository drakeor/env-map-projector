#ifndef CONVERSION_CONTROLLER_H
#define CONVERSION_CONTROLLER_H

#include <vector>

#include "GuiState.h"
#include "ImageSlot.h"

class ConversionController
{
public:
    ConversionController() = default;

    void Convert(GuiState& state, const std::vector<ImageSlot>& slots);

private:
    std::vector<EnvMapImage> PrepareImages(const std::vector<ImageSlot>& slots, AutoScaleMode mode);
    EnvMapImage ResampleImage(const EnvMapImage& image, int targetWidth, int targetHeight);
};

#endif
