#ifndef INPUT_PANEL_H
#define INPUT_PANEL_H

#include <vector>

#include <imgui.h>

#include "GuiState.h"
#include "ImageSlot.h"

class InputPanel
{
public:
    InputPanel();

    void Draw(GuiState& state);

    bool HasAllImages() const;
    const std::vector<ImageSlot>& GetSlots() const;

private:
    void RebuildSlots(ProjectionType type);
    void DrawProjectionSelector(GuiState& state);
    void DrawPreviewModal();

    ProjectionType currentProjection;
    std::vector<ImageSlot> slots;
    bool previewOpen;
    int previewSlotIndex;
    bool openPreviewThisFrame;
};

#endif
