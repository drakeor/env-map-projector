#include "OutputPanel.h"

#include <algorithm>
#include <filesystem>
#include <string>

#include <GLFW/glfw3.h>

#include "Utils/ImageWriter.h"

OutputPanel::OutputPanel()
    : cachedGeneration(0)
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
    if(state.outputProjection == ProjectionType::Equirectangular)
    {
        ImGui::InputInt("Output width", &state.outputEquirectWidth);
        ImGui::InputInt("Output height", &state.outputEquirectHeight);
        state.outputEquirectWidth = std::max(16, state.outputEquirectWidth);
        state.outputEquirectHeight = std::max(16, state.outputEquirectHeight);
    }
    else
    {
        ImGui::InputInt("Output side length", &state.outputCubeSide);
        state.outputCubeSide = std::max(16, state.outputCubeSide);
    }

    ImGui::InputText("Output directory", state.outputDirectory.data(), state.outputDirectory.size());
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
            state.conversionInProgress = true;
            controller.Convert(state, inputPanel.GetSlots());
            state.conversionInProgress = false;
        }
    }
    ImGui::SameLine();
    if(ImGui::Button("Save outputs"))
    {
        SaveOutputs(state);
    }

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

    for(size_t i = 0; i < state.convertedImages.size() && i < outputTextures.size(); ++i)
    {
        ImGui::TextUnformatted(state.convertedLabels[i].c_str());
        GLuint tex = outputTextures[i];
        if(tex != 0)
        {
            ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(tex)), ImVec2(200.0f, 200.0f));
        }
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
