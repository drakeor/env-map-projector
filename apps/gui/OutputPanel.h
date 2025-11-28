#ifndef OUTPUT_PANEL_H
#define OUTPUT_PANEL_H

#include <string>
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
    void DrawOutputPreviewModal(GuiState& state);
    void DrawSaveOutputsModal();
    bool SaveOutputs(GuiState& state, std::string& savedDirectory);
    void RebuildOutputTextures(GuiState& state);
    void ReleaseOutputTextures();

    std::vector<GLuint> outputTextures;
    uint64_t cachedGeneration;
    bool outputPreviewOpen;
    int outputPreviewIndex;
    bool openOutputPreviewThisFrame;
    bool saveOutputsModalOpen;
    bool openSaveOutputsModalThisFrame;
    std::string lastSavedDirectory;
};

#endif
