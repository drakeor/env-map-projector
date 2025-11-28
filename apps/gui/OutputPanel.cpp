#include "OutputPanel.h"

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <string>

#include <GLFW/glfw3.h>

#include "FileDialog.h"
#include "Utils/ImageWriter.h"

namespace
{
ImVec2 FitImageSize(int width, int height, float maxEdge)
{
    if(width <= 0 || height <= 0)
        return ImVec2(maxEdge, maxEdge);

    float w = static_cast<float>(width);
    float h = static_cast<float>(height);
    float aspect = w / h;
    if(aspect >= 1.0f)
        return ImVec2(maxEdge, maxEdge / aspect);
    return ImVec2(maxEdge * aspect, maxEdge);
}

void DrawSizeLabel(const char* label, int value)
{
    if(value > 0)
        ImGui::Text("%s: %d px", label, value);
    else
        ImGui::Text("%s: auto", label);
}
}

OutputPanel::OutputPanel()
    : cachedGeneration(0), outputPreviewOpen(false), outputPreviewIndex(-1), openOutputPreviewThisFrame(false)
{
}

OutputPanel::~OutputPanel()
{
    ReleaseOutputTextures();
}

void OutputPanel::Draw(GuiState& state, InputPanel& inputPanel, ConversionController& controller)
{
    DrawProjectionSelector(state);
    DrawAutoScaleSelector(state);
    DrawOutputSettings(state);
    DrawConversionControls(state, inputPanel, controller);
    DrawConvertedOutputs(state);
}

void OutputPanel::DrawProjectionSelector(GuiState& state)
{
    const char* options[] = {"Equirectangular", "Skybox", "Hemispherical"};
    int current = static_cast<int>(state.outputProjection);
    ImGui::Combo("Output Projection", &current, options, IM_ARRAYSIZE(options));
    state.outputProjection = static_cast<ProjectionType>(current);
}

void OutputPanel::DrawAutoScaleSelector(GuiState& state)
{
    const char* modes[] = {"No scaling", "Scale to largest", "Scale to smallest"};
    int current = static_cast<int>(state.autoScaleMode);
    ImGui::Combo("Input scaling", &current, modes, IM_ARRAYSIZE(modes));
    state.autoScaleMode = static_cast<AutoScaleMode>(current);
}

void OutputPanel::DrawOutputSettings(GuiState& state)
{
    static const char* scaleLabels[] = {"1.0x", "2.0x", "4.0x"};
    static const float scaleValues[] = {1.0f, 2.0f, 4.0f};
    state.uiScaleIndex = std::clamp(state.uiScaleIndex, 0, 2);
    int scaleIndex = state.uiScaleIndex;
    if(ImGui::Combo("UI scale", &scaleIndex, scaleLabels, IM_ARRAYSIZE(scaleLabels)))
    {
        state.uiScaleIndex = scaleIndex;
    }
    state.uiScale = scaleValues[state.uiScaleIndex];

    if(ImGui::SliderFloat("Output scale", &state.outputScale, 0.5f, 4.0f, "%.1fx"))
    {
        state.outputScale = std::clamp(state.outputScale, 0.5f, 4.0f);
    }

    ImGui::SeparatorText("Output size");
    if(state.outputProjection == ProjectionType::Equirectangular)
    {
        DrawSizeLabel("Width", state.outputEquirectWidth);
        DrawSizeLabel("Height", state.outputEquirectHeight);
    }
    else if(state.outputProjection == ProjectionType::Skybox)
    {
        DrawSizeLabel("Face size", state.outputSkyboxSize);
    }
    else
    {
        DrawSizeLabel("Hemisphere size", state.outputHemisphericalSize);
    }

    ImGui::InputText("Output directory", state.outputDirectory.data(), state.outputDirectory.size());
    ImGui::SameLine();
    if(ImGui::Button("Browse##outputDir"))
    {
        std::string selected = FileDialog::SelectDirectory(state.outputDirectory.data());
        if(!selected.empty())
        {
            std::snprintf(state.outputDirectory.data(), state.outputDirectory.size(), "%s", selected.c_str());
        }
    }
    ImGui::InputText("Filename prefix", state.outputPrefix.data(), state.outputPrefix.size());
}

void OutputPanel::DrawConversionControls(GuiState& state, InputPanel& inputPanel, ConversionController& controller)
{
    if(ImGui::Button("Convert"))
    {
        if(!inputPanel.HasAllImages())
        {
            state.statusText = "Load all required inputs";
        }
        else if(!state.conversionInProgress)
        {
            controller.StartConversion(state, inputPanel.GetSlots());
        }
    }
    ImGui::SameLine();
    if(ImGui::Button("Save outputs"))
    {
        SaveOutputs(state);
    }

    ImGui::ProgressBar(state.conversionProgress, ImVec2(-1.0f, 0.0f));

    if(!state.statusText.empty())
    {
        ImGui::TextUnformatted(state.statusText.c_str());
    }
}

void OutputPanel::DrawConvertedOutputs(GuiState& state)
{
    if(state.convertedImages.empty())
        return;

    if(cachedGeneration != state.convertedGeneration)
    {
        RebuildOutputTextures(state);
    }

    openOutputPreviewThisFrame = false;

    for(size_t i = 0; i < state.convertedImages.size() && i < outputTextures.size(); ++i)
    {
        ImGui::TextUnformatted(state.convertedLabels[i].c_str());
        const EnvMapImage& image = state.convertedImages[i];
        GLuint tex = outputTextures[i];
        if(tex != 0)
        {
            ImVec2 previewSize = FitImageSize(image.GetWidth(), image.GetHeight(), 220.0f);
            ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(tex)), previewSize);
            if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                outputPreviewIndex = static_cast<int>(i);
                outputPreviewOpen = true;
                openOutputPreviewThisFrame = true;
            }
        }
    }

    if(openOutputPreviewThisFrame)
    {
        ImGui::OpenPopup("OutputPreviewModal");
    }

    DrawOutputPreviewModal(state);
}

void OutputPanel::DrawOutputPreviewModal(GuiState& state)
{
    if(!outputPreviewOpen)
        return;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 viewportSize = viewport ? viewport->Size : ImVec2(1280.0f, 720.0f);
    ImVec2 modalSize(viewportSize.x * 0.75f, viewportSize.y * 0.75f);
    ImGui::SetNextWindowSize(modalSize, ImGuiCond_Appearing);

    bool modalOpen = true;
    if(ImGui::BeginPopupModal("OutputPreviewModal", &modalOpen, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if(outputPreviewIndex < 0 || outputPreviewIndex >= static_cast<int>(state.convertedImages.size()))
        {
            ImGui::TextUnformatted("No image available");
        }
        else
        {
            const EnvMapImage& image = state.convertedImages[outputPreviewIndex];
            std::string label = state.convertedLabels[outputPreviewIndex];
            ImGui::TextUnformatted(label.c_str());
            GLuint tex = (outputPreviewIndex < static_cast<int>(outputTextures.size())) ?
                outputTextures[outputPreviewIndex] : 0;
            if(tex != 0)
            {
                ImVec2 avail = ImGui::GetContentRegionAvail();
                float maxEdge = std::min(avail.x, avail.y);
                if(maxEdge <= 0.0f)
                    maxEdge = std::min(modalSize.x, modalSize.y) - 40.0f;
                float texW = static_cast<float>(image.GetWidth());
                float texH = static_cast<float>(image.GetHeight());
                float scale = 1.0f;
                if(texW > maxEdge || texH > maxEdge)
                {
                    float scaleW = maxEdge / texW;
                    float scaleH = maxEdge / texH;
                    scale = std::min(scaleW, scaleH);
                }
                ImVec2 size(texW * scale, texH * scale);
                ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(tex)), size);
            }
        }

        if(ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
            modalOpen = false;
        }
        ImGui::EndPopup();
    }

    if(!modalOpen)
    {
        outputPreviewOpen = false;
        outputPreviewIndex = -1;
    }
}

void OutputPanel::SaveOutputs(GuiState& state)
{
    if(state.convertedImages.empty())
    {
        state.statusText = "Nothing to save";
        return;
    }

    std::string directory(state.outputDirectory.data());
    if(directory.empty())
    {
        state.statusText = "Specify output directory";
        return;
    }

    std::filesystem::create_directories(directory);

    ImageWriter writer;
    std::string prefix(state.outputPrefix.data());
    if(prefix.empty())
        prefix = "converted";
    for(size_t i = 0; i < state.convertedImages.size(); ++i)
    {
        std::string filename = directory + "/" + prefix + "_" + state.convertedLabels[i] + ".png";
        writer.SaveImage(filename, state.convertedImages[i]);
    }

    state.statusText = "Saved outputs";
}

void OutputPanel::RebuildOutputTextures(GuiState& state)
{
    ReleaseOutputTextures();
    for(EnvMapImage& image : state.convertedImages)
    {
        GLuint tex = 0;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.GetWidth(), image.GetHeight(), 0,
            GL_RGBA, GL_UNSIGNED_BYTE, image.GetData().get());
        glBindTexture(GL_TEXTURE_2D, 0);
        outputTextures.push_back(tex);
    }
    cachedGeneration = state.convertedGeneration;
}

void OutputPanel::ReleaseOutputTextures()
{
    for(GLuint tex : outputTextures)
    {
        if(tex != 0)
        {
            glDeleteTextures(1, &tex);
        }
    }
    outputTextures.clear();
}
