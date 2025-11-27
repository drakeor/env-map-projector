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

ConversionController::ConversionController()
    : running(false), progress(0.0f)
{
}

ConversionController::~ConversionController()
{
    JoinWorker();
}

void ConversionController::Update(GuiState& state)
{
    state.conversionInProgress = running.load();
    state.conversionProgress = progress.load();

    std::unique_ptr<PendingResult> result;
    {
        std::lock_guard<std::mutex> lock(resultMutex);
        if(pendingResult)
        {
            result = std::move(pendingResult);
        }
    }

    if(result)
    {
        if(result->success)
        {
            state.convertedImages = std::move(result->images);
            state.convertedLabels = std::move(result->labels);
            state.convertedGeneration++;
            state.conversionProgress = 1.0f;
        }
        state.statusText = result->message;
    }
}

bool ConversionController::StartConversion(GuiState& state, const std::vector<ImageSlot>& slots)
{
    if(running.load())
    {
        state.statusText = "Conversion already running";
        return false;
    }

    if(slots.empty())
    {
        state.statusText = "No inputs provided";
        return false;
    }

    std::vector<SlotPayload> payloads;
    payloads.reserve(slots.size());
    for(const ImageSlot& slot : slots)
    {
        const EnvMapImage* img = slot.GetImage();
        if(!img)
        {
            state.statusText = "Load all required inputs";
            return false;
        }
        payloads.push_back({slot.GetLabel(), *img});
    }

    ConversionParams params;
    params.inputProjection = state.inputProjection;
    params.outputProjection = state.outputProjection;
    params.autoScaleMode = state.autoScaleMode;
    params.outputWidth = state.outputEquirectWidth;
    params.outputHeight = state.outputEquirectHeight;
    params.cubeSide = state.outputCubeSide;

    JoinWorker();
    {
        std::lock_guard<std::mutex> lock(resultMutex);
        pendingResult.reset();
    }

    running = true;
    progress = 0.01f;
    state.conversionInProgress = true;
    state.conversionProgress = 0.0f;
    state.statusText = "Converting...";

    worker = std::thread(&ConversionController::WorkerThread, this, params, std::move(payloads));
    return true;
}

void ConversionController::WorkerThread(ConversionParams params, std::vector<SlotPayload> inputs)
{
    std::unique_ptr<PendingResult> result = std::make_unique<PendingResult>();

    std::vector<EnvMapImage> images;
    images.reserve(inputs.size());
    for(const SlotPayload& payload : inputs)
    {
        images.push_back(payload.image);
    }

    progress = 0.15f;
    images = PrepareImages(std::move(images), params.autoScaleMode);
    if(images.empty())
    {
        result->success = false;
        result->message = "Input preparation failed";
        {
            std::lock_guard<std::mutex> lock(resultMutex);
            pendingResult = std::move(result);
        }
        progress = 0.0f;
        running = false;
        return;
    }

    std::shared_ptr<EnvProj::CoordContainerBase<double>> coords;
    switch(params.inputProjection)
    {
        case ProjectionType::Equirectangular:
        {
            EnvProj::EquirectangularProjection<double> proj;
            coords = proj.LoadImageToSphericalCoords(&images[0]);
            break;
        }
        case ProjectionType::Skybox:
        {
            EnvProj::SkyboxProjection<double> proj;
            coords = proj.LoadImageToSphericalCoords(
                &images[0], &images[1], &images[2], &images[3], &images[4], &images[5]);
            break;
        }
        case ProjectionType::Hemispherical:
        {
            EnvProj::HemisphericalProjection<double> proj;
            coords = proj.LoadImageToSphericalCoords(&images[0], &images[1]);
            break;
        }
    }

    if(!coords)
    {
        result->success = false;
        result->message = "Coordinate conversion failed";
        {
            std::lock_guard<std::mutex> lock(resultMutex);
            pendingResult = std::move(result);
        }
        progress = 0.0f;
        running = false;
        return;
    }

    progress = 0.55f;
    std::vector<EnvMapImage> outputs;
    std::vector<std::string> labels;

    switch(params.outputProjection)
    {
        case ProjectionType::Equirectangular:
        {
            EnvProj::EquirectangularProjection<double> proj;
            outputs.push_back(proj.ConvertToImage(coords.get(), params.outputWidth, params.outputHeight));
            labels.push_back("equirect");
            break;
        }
        case ProjectionType::Skybox:
        {
            EnvProj::SkyboxProjection<double> proj;
            auto imgs = proj.ConvertToImages(coords.get(), static_cast<uint32_t>(params.cubeSide));
            auto lbls = SkyboxLabels();
            for(size_t i = 0; i < imgs.size(); ++i)
            {
                outputs.push_back(imgs[i]);
                labels.push_back(lbls[i]);
            }
            break;
        }
        case ProjectionType::Hemispherical:
        {
            EnvProj::HemisphericalProjection<double> proj;
            auto imgs = proj.ConvertToImages(coords.get(), static_cast<uint32_t>(params.cubeSide));
            auto lbls = HemisphereLabels();
            for(size_t i = 0; i < imgs.size(); ++i)
            {
                outputs.push_back(imgs[i]);
                labels.push_back(lbls[i]);
            }
            break;
        }
    }

    result->success = true;
    result->images = std::move(outputs);
    result->labels = std::move(labels);
    result->message = "Conversion complete";
    progress = 1.0f;

    {
        std::lock_guard<std::mutex> lock(resultMutex);
        pendingResult = std::move(result);
    }

    running = false;
}

std::vector<EnvMapImage> ConversionController::PrepareImages(std::vector<EnvMapImage> images, AutoScaleMode mode)
{
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

void ConversionController::JoinWorker()
{
    if(worker.joinable())
    {
        worker.join();
    }
    running = false;
    progress = 0.0f;
}
