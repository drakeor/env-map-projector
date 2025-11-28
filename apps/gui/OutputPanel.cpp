#include "OutputPanel.h"

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <string>

#include <GLFW/glfw3.h>

#include "FileDialog.h"
#include "Utils/ImageWriter.h"

#define GL_CLAMP_TO_EDGE 0x812F

namespace
{
constexpr float kThumbnailEdge = 220.0f;

ImVec2 ComputeThumbnailSize(int width, int height)
{
    if(width <= 0 || height <= 0)
        return ImVec2(kThumbnailEdge, kThumbnailEdge);

    float w = static_cast<float>(width);
    float h = static_cast<float>(height);
    float aspect = w / h;
    if(aspect >= 1.0f)
        return ImVec2(kThumbnailEdge, kThumbnailEdge / aspect);
    return ImVec2(kThumbnailEdge * aspect, kThumbnailEdge);
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
    : cachedGeneration(0), outputPreviewOpen(false), outputPreviewIndex(-1), openOutputPreviewThisFrame(false),
      saveOutputsModalOpen(false), openSaveOutputsModalThisFrame(false)
{
}

OutputPanel::~OutputPanel()
{
    ReleaseOutputTextures();
}

void OutputPanel::Draw(GuiState& state, InputPanel& inputPanel, ConversionController& controller)
{
    openSaveOutputsModalThisFrame = false;

    DrawProjectionSelector(state);

    ImGui::SeparatorText("Output Options");
    DrawAutoScaleSelector(state);
    DrawOutputSettings(state);

    ImGui::SeparatorText("Output Controls");
    DrawConversionControls(state, inputPanel, controller);

    ImGui::SeparatorText("Output Previews");
    DrawConvertedOutputs(state);

    DrawSaveOutputsModal();
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

    //ImGui::SameLine();
    ImGui::InputText("Filename prefix", state.outputPrefix.data(), state.outputPrefix.size());

    ImGui::InputText("Output directory", state.outputDirectory.data(), state.outputDirectory.size());

    if(ImGui::Button("Browse##outputDir"))
    {
        std::string selected = FileDialog::SelectDirectory(state.outputDirectory.data());
        if(!selected.empty())
        {
            std::snprintf(state.outputDirectory.data(), state.outputDirectory.size(), "%s", selected.c_str());
        }
    }
}

void OutputPanel::DrawConversionControls(GuiState& state, InputPanel& inputPanel, ConversionController& controller)
{

    // Display output size info
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
        std::string savedDir;
        if(SaveOutputs(state, savedDir))
        {
            lastSavedDirectory = savedDir;
            saveOutputsModalOpen = true;
            openSaveOutputsModalThisFrame = true;
        }
    }

    ImGui::ProgressBar(state.conversionProgress, ImVec2(-1.0f, 0.0f));

    if(!state.statusText.empty())
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.95f, 0.75f, 1.0f));
        ImGui::TextWrapped("%s", state.statusText.c_str());
        ImGui::PopStyleColor();
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
            ImVec2 previewSize = ComputeThumbnailSize(image.GetWidth(), image.GetHeight());
            ImGui::Image(static_cast<ImTextureID>(tex), previewSize);
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
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal("OutputPreviewModal", &modalOpen, flags))
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
                if(avail.x <= 0.0f || avail.y <= 0.0f)
                {
                    avail.x = modalSize.x - 40.0f;
                    avail.y = modalSize.y - 80.0f;
                }
                float texW = static_cast<float>(image.GetWidth());
                float texH = static_cast<float>(image.GetHeight());
                float scaleW = avail.x / texW;
                float scaleH = avail.y / texH;
                float scale = std::min(scaleW, scaleH);
                if(scale > 1.0f)
                    scale = 1.0f;
                if(scale <= 0.0f)
                    scale = 1.0f;
                ImVec2 size(texW * scale, texH * scale);
                ImGui::Image(static_cast<ImTextureID>(tex), size);
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

void OutputPanel::DrawSaveOutputsModal()
{
    if(openSaveOutputsModalThisFrame)
    {
        ImGui::OpenPopup("OutputsSavedModal");
    }

    if(!saveOutputsModalOpen)
        return;

    bool modalOpen = true;
    if(ImGui::BeginPopupModal("OutputsSavedModal", &modalOpen, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if(lastSavedDirectory.empty())
        {
            ImGui::TextUnformatted("Outputs saved.");
        }
        else
        {
            ImGui::Text("Outputs saved to:\n%s", lastSavedDirectory.c_str());
        }

        if(ImGui::Button("OK"))
        {
            ImGui::CloseCurrentPopup();
            modalOpen = false;
        }
        ImGui::EndPopup();
    }

    if(!modalOpen)
    {
        saveOutputsModalOpen = false;
    }
}

bool OutputPanel::SaveOutputs(GuiState& state, std::string& savedDirectory)
{
    savedDirectory.clear();
    if(state.convertedImages.empty())
    {
        state.statusText = "Nothing to save";
        return false;
    }

    std::string directory(state.outputDirectory.data());
    if(directory.empty())
    {
        state.statusText = "Specify output directory";
        return false;
    }

    std::filesystem::path outputPath(directory);
    std::error_code ec;
    std::filesystem::create_directories(outputPath, ec);

    ImageWriter writer;
    std::string prefix(state.outputPrefix.data());
    if(prefix.empty())
        prefix = "converted";
    for(size_t i = 0; i < state.convertedImages.size(); ++i)
    {
        std::filesystem::path filename = outputPath / (prefix + "_" + state.convertedLabels[i] + ".png");
        writer.SaveImage(filename.string(), state.convertedImages[i]);
    }

    savedDirectory = std::filesystem::absolute(outputPath).string();
    state.statusText = "Saved outputs to " + savedDirectory;
    return true;
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
