#ifndef CONVERSION_CONTROLLER_H
#define CONVERSION_CONTROLLER_H

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "GuiState.h"
#include "ImageSlot.h"

class ConversionController
{
public:
    ConversionController();
    ~ConversionController();

    void Update(GuiState& state);
    bool StartConversion(GuiState& state, const std::vector<ImageSlot>& slots);

private:
    struct SlotPayload
    {
        std::string label;
        EnvMapImage image;
    };

    struct ConversionParams
    {
        ProjectionType inputProjection;
        ProjectionType outputProjection;
        AutoScaleMode autoScaleMode;
        float outputScale;
        int outputWidth;
        int outputHeight;
    };

    struct PendingResult
    {
        bool success = false;
        std::vector<EnvMapImage> images;
        std::vector<std::string> labels;
        std::string message;
        ProjectionType outputProjection = ProjectionType::Equirectangular;
        int outputWidth = 0;
        int outputHeight = 0;
    };

    void WorkerThread(ConversionParams params, std::vector<SlotPayload> inputs);
    std::vector<EnvMapImage> PrepareImages(std::vector<EnvMapImage> images, AutoScaleMode mode);
    EnvMapImage ResampleImage(const EnvMapImage& image, int targetWidth, int targetHeight);

    void JoinWorker();

    std::atomic<bool> running;
    std::atomic<float> progress;
    std::thread worker;
    std::mutex resultMutex;
    std::unique_ptr<PendingResult> pendingResult;
};

#endif
