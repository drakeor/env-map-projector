#include "InputPanel.h"

#include <algorithm>
#include <string>

#include "FileDialog.h"

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
}

InputPanel::InputPanel()
    : currentProjection(ProjectionType::Equirectangular), previewOpen(false), previewSlotIndex(-1), openPreviewThisFrame(false)
{
    RebuildSlots(currentProjection);
}

void InputPanel::Draw(GuiState& state)
{
    DrawProjectionSelector(state);

    ImGui::SeparatorText("Input Images");

    openPreviewThisFrame = false;

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
                std::string base = slot.GetPathString();
                std::string selected = FileDialog::OpenImageFile(base);
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
                const ImVec4 transformColor = ImVec4(0.20f, 0.32f, 0.58f, 1.0f);
                const ImVec4 transformHover = ImVec4(0.27f, 0.41f, 0.69f, 1.0f);
                const ImVec4 transformActive = ImVec4(0.16f, 0.29f, 0.52f, 1.0f);
                ImGui::PushStyleColor(ImGuiCol_Button, transformColor);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, transformHover);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, transformActive);
                if(ImGui::Button("Rotate CW"))
                    slot.Rotate90CW();
                ImGui::SameLine();
                if(ImGui::Button("Rotate CCW"))
                    slot.Rotate90CCW();
                ImGui::SameLine();
                if(ImGui::Button("Flip H"))
                    slot.FlipHorizontal();
                ImGui::SameLine();
                if(ImGui::Button("Flip V"))
                    slot.FlipVertical();
                ImGui::PopStyleColor(3);

                ImGui::Text("%dx%d", slot.GetWidth(), slot.GetHeight());
                if(slot.GetTextureId() != 0)
                {
                    ImVec2 previewSize = ComputeThumbnailSize(slot.GetWidth(), slot.GetHeight());
                    ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(slot.GetTextureId())), previewSize);
                    if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    {
                        previewSlotIndex = static_cast<int>(i);
                        previewOpen = true;
                        openPreviewThisFrame = true;
                    }
                }
            }
            else
            {
                ImGui::TextUnformatted("No image loaded");
            }
            ImGui::PopID();
        }
    }

    if(openPreviewThisFrame)
    {
        ImGui::OpenPopup("ImagePreviewModal");
    }

    DrawPreviewModal();
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

    if(state.inputProjection == ProjectionType::Hemispherical)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.6f, 0.1f, 1.0f));
        ImGui::TextWrapped("Warning: Hemispherical conversion is experimental and may introduce visible artifacts in output images.");
        ImGui::PopStyleColor();
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

void InputPanel::DrawPreviewModal()
{
    if(!previewOpen)
        return;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 viewportSize = viewport ? viewport->Size : ImVec2(1280.0f, 720.0f);
    ImVec2 modalSize(viewportSize.x * 0.75f, viewportSize.y * 0.75f);
    ImGui::SetNextWindowSize(modalSize, ImGuiCond_Appearing);

    bool modalOpen = true;
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal("ImagePreviewModal", &modalOpen, flags))
    {
            if(previewSlotIndex < 0 || previewSlotIndex >= static_cast<int>(slots.size()) || !slots[previewSlotIndex].HasImage())
            {
                ImGui::TextUnformatted("No image available");
            }
            else
            {
                const ImageSlot& slot = slots[previewSlotIndex];
                ImGui::TextUnformatted(slot.GetLabel().c_str());
                if(slot.GetTextureId() != 0)
                {
                    ImVec2 avail = ImGui::GetContentRegionAvail();
                    if(avail.x <= 0.0f || avail.y <= 0.0f)
                    {
                        avail.x = modalSize.x - 40.0f;
                        avail.y = modalSize.y - 80.0f;
                    }
                    float texW = static_cast<float>(slot.GetWidth());
                    float texH = static_cast<float>(slot.GetHeight());
                    float scaleW = avail.x / texW;
                    float scaleH = avail.y / texH;
                    float scale = std::min(scaleW, scaleH);
                    if(scale > 1.0f)
                        scale = 1.0f;
                    if(scale <= 0.0f)
                        scale = 1.0f;
                    ImVec2 size(texW * scale, texH * scale);
                    ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(slot.GetTextureId())), size);
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
        previewOpen = false;
        previewSlotIndex = -1;
    }
}
