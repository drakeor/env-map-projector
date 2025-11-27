#ifndef IMAGE_SLOT_H
#define IMAGE_SLOT_H

#include <array>
#include <memory>
#include <string>

#include <GLFW/glfw3.h>

#include "Utils/EnvMapImage.h"

class ImageSlot
{
public:
    static constexpr size_t PathBufferSize = 1024;

    explicit ImageSlot(const std::string& label);
    ~ImageSlot();

    const std::string& GetLabel() const;
    void SetPath(const std::string& path);
    std::string GetPathString() const;
    char* GetPathBuffer();
    const char* GetPathBuffer() const;

    bool LoadFromDisk();
    void Clear();

    bool HasImage() const;
    int GetWidth() const;
    int GetHeight() const;
    const EnvMapImage* GetImage() const;

    GLuint GetTextureId() const;

    bool Rotate90CW();
    bool Rotate90CCW();
    bool FlipHorizontal();
    bool FlipVertical();

private:
    void UploadTexture();
    void ReleaseTexture();
    std::string label;
    std::array<char, PathBufferSize> pathBuffer;
    std::shared_ptr<EnvMapImage> image;
    GLuint textureId;
};

#endif
