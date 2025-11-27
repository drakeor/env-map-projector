#include "InputPanel.h"

#include <string>

#include "FileDialog.h"

InputPanel::InputPanel()
    : currentProjection(ProjectionType::Equirectangular)
{
    RebuildSlots(currentProjection);
}

void InputPanel::Draw(GuiState& state)
{
    DrawProjectionSelector(state);

    for(size_t i = 0; i < slots.size(); ++i)
    {
        ImageSlot& slot = slots[i];

        std::string header = slot.GetLabel() + "##slot" + std::to_string(i);
        if(ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::PushID(static_cast<int>(i));
            ImGui::InputText("Path", slot.GetPathBuffer(), ImageSlot::PathBufferSize);
            ImGui::SameLine();
            if(ImGui::Button("Browse"))
            {
                std::string selected = FileDialog::OpenImageFile(slot.GetPathString());
                if(!selected.empty())
                {
                    slot.SetPath(selected);
                    slot.LoadFromDisk();
                }
            }

            if(ImGui::Button("Load"))
            {
                slot.LoadFromDisk();
            }
            ImGui::SameLine();
            if(ImGui::Button("Clear"))
            {
                slot.Clear();
            }

            if(slot.HasImage())
            {
                ImGui::Text("%dx%d", slot.GetWidth(), slot.GetHeight());
                if(slot.GetTextureId() != 0)
                {
                    ImVec2 size(200.0f, 200.0f);
                    ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(slot.GetTextureId())), size);
                }
            }
            else
            {
                ImGui::TextUnformatted("No image loaded");
            }
            ImGui::PopID();
        }
    }
}

bool InputPanel::HasAllImages() const
{
    for(const ImageSlot& slot : slots)
    {
        if(!slot.HasImage())
            return false;
    }
    return !slots.empty();
}

const std::vector<ImageSlot>& InputPanel::GetSlots() const
{
    return slots;
}

void InputPanel::DrawProjectionSelector(GuiState& state)
{
    const char* options[] = {"Equirectangular", "Skybox", "Hemispherical"};
    int current = static_cast<int>(state.inputProjection);
    if(ImGui::Combo("Input Projection", &current, options, IM_ARRAYSIZE(options)))
    {
        state.inputProjection = static_cast<ProjectionType>(current);
        if(state.inputProjection != currentProjection)
        {
            currentProjection = state.inputProjection;
            RebuildSlots(currentProjection);
        }
    }
}

void InputPanel::RebuildSlots(ProjectionType type)
{
    slots.clear();

    switch(type)
    {
        case ProjectionType::Equirectangular:
            slots.emplace_back("Equirectangular Map");
            break;
        case ProjectionType::Skybox:
            slots.emplace_back("Top");
            slots.emplace_back("Bottom");
            slots.emplace_back("Left");
            slots.emplace_back("Right");
            slots.emplace_back("Front");
            slots.emplace_back("Back");
            break;
        case ProjectionType::Hemispherical:
            slots.emplace_back("Top Hemisphere");
            slots.emplace_back("Bottom Hemisphere");
            break;
    }
}
