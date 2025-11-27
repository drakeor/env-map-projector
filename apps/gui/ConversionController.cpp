#include "ConversionController.h"

#include <algorithm>
#include <array>
#include <memory>
#include <string>

#include "Projections/EquirectangularProjection.h"
#include "Projections/SkyboxProjection.h"
#include "Projections/HemisphericalProjection.h"

namespace
{
std::vector<std::string> SkyboxLabels()
{
    return {"top", "bottom", "left", "right", "front", "back"};
}

std::vector<std::string> HemisphereLabels()
{
    return {"top", "bottom"};
}
}

void ConversionController::Convert(GuiState& state, const std::vector<ImageSlot>& slots)
{
    state.statusText.clear();

    if(slots.empty())
    {
        state.statusText = "No inputs provided";
        return;
    }

    std::vector<EnvMapImage> prepared = PrepareImages(slots, state.autoScaleMode);
    if(prepared.size() != slots.size())
    {
        state.statusText = "Input preparation failed";
        return;
    }

    std::shared_ptr<EnvProj::CoordContainerBase<double>> coords;
    switch(state.inputProjection)
    {
        case ProjectionType::Equirectangular:
        {
            EnvProj::EquirectangularProjection<double> proj;
            coords = proj.LoadImageToSphericalCoords(&prepared[0]);
            break;
        }
        case ProjectionType::Skybox:
        {
            EnvProj::SkyboxProjection<double> proj;
            coords = proj.LoadImageToSphericalCoords(
                &prepared[0], &prepared[1], &prepared[2], &prepared[3], &prepared[4], &prepared[5]);
            break;
        }
        case ProjectionType::Hemispherical:
        {
            EnvProj::HemisphericalProjection<double> proj;
            coords = proj.LoadImageToSphericalCoords(&prepared[0], &prepared[1]);
            break;
        }
    }

    if(!coords)
    {
        state.statusText = "Coordinate conversion failed";
        return;
    }

    state.convertedImages.clear();
    state.convertedLabels.clear();

    switch(state.outputProjection)
    {
        case ProjectionType::Equirectangular:
        {
            EnvProj::EquirectangularProjection<double> proj;
            EnvMapImage image = proj.ConvertToImage(coords.get(), state.outputEquirectWidth, state.outputEquirectHeight);
            state.convertedImages.push_back(image);
            state.convertedLabels.push_back("equirect");
            break;
        }
        case ProjectionType::Skybox:
        {
            EnvProj::SkyboxProjection<double> proj;
            auto images = proj.ConvertToImages(coords.get(), static_cast<uint32_t>(state.outputCubeSide));
            auto labels = SkyboxLabels();
            for(size_t i = 0; i < images.size(); ++i)
            {
                state.convertedImages.push_back(images[i]);
                state.convertedLabels.push_back(labels[i]);
            }
            break;
        }
        case ProjectionType::Hemispherical:
        {
            EnvProj::HemisphericalProjection<double> proj;
            auto images = proj.ConvertToImages(coords.get(), static_cast<uint32_t>(state.outputCubeSide));
            auto labels = HemisphereLabels();
            for(size_t i = 0; i < images.size(); ++i)
            {
                state.convertedImages.push_back(images[i]);
                state.convertedLabels.push_back(labels[i]);
            }
            break;
        }
    }

    state.convertedGeneration++;
    state.statusText = "Conversion successful";
}

std::vector<EnvMapImage> ConversionController::PrepareImages(const std::vector<ImageSlot>& slots, AutoScaleMode mode)
{
    std::vector<EnvMapImage> images;
    images.reserve(slots.size());
    for(const ImageSlot& slot : slots)
    {
        const EnvMapImage* img = slot.GetImage();
        if(!img)
            return {};
        images.push_back(*img);
    }

    if(mode == AutoScaleMode::None || images.size() <= 1)
        return images;

    int targetWidth = images[0].GetWidth();
    int targetHeight = images[0].GetHeight();

    for(const EnvMapImage& img : images)
    {
        if(mode == AutoScaleMode::ScaleToLargest)
        {
            targetWidth = std::max(targetWidth, img.GetWidth());
            targetHeight = std::max(targetHeight, img.GetHeight());
        }
        else if(mode == AutoScaleMode::ScaleToSmallest)
        {
            targetWidth = std::min(targetWidth, img.GetWidth());
            targetHeight = std::min(targetHeight, img.GetHeight());
        }
    }

    for(EnvMapImage& img : images)
    {
        if(img.GetWidth() != targetWidth || img.GetHeight() != targetHeight)
        {
            img = ResampleImage(img, targetWidth, targetHeight);
        }
    }

    return images;
}

EnvMapImage ConversionController::ResampleImage(const EnvMapImage& image, int targetWidth, int targetHeight)
{
    EnvMapImage scaled(targetWidth, targetHeight);

    for(int y = 0; y < targetHeight; ++y)
    {
        float v = static_cast<float>(y) / static_cast<float>(targetHeight - 1);
        int srcY = static_cast<int>(v * (image.GetHeight() - 1));
        for(int x = 0; x < targetWidth; ++x)
        {
            float u = static_cast<float>(x) / static_cast<float>(targetWidth - 1);
            int srcX = static_cast<int>(u * (image.GetWidth() - 1));
            unsigned int pixel = image.GetPixel(srcX, srcY);
            scaled.SetPixel(x, y, pixel);
        }
    }

    return scaled;
}
