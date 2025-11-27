#ifndef OUTPUT_PANEL_H
#define OUTPUT_PANEL_H

#include <vector>

#include <imgui.h>

#include "GuiState.h"
#include "InputPanel.h"
#include "ConversionController.h"

class OutputPanel
{
public:
    OutputPanel();
    ~OutputPanel();

    void Draw(GuiState& state, InputPanel& inputPanel, ConversionController& controller);

private:
    void DrawProjectionSelector(GuiState& state);
    void DrawAutoScaleSelector(GuiState& state);
    void DrawConversionControls(GuiState& state, InputPanel& inputPanel, ConversionController& controller);
    void DrawOutputSettings(GuiState& state);
    void DrawConvertedOutputs(GuiState& state);
    void SaveOutputs(GuiState& state);
    void RebuildOutputTextures(GuiState& state);
    void ReleaseOutputTextures();

    std::vector<GLuint> outputTextures;
    uint64_t cachedGeneration;
};

#endif
