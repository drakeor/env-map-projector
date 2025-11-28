#include "ImageSlot.h"

#include <cstdio>
#include <cstring>
#include <iostream>

#include "Utils/ImageReader.h"

#define GL_CLAMP_TO_EDGE 0x812F

ImageSlot::ImageSlot(const std::string& _label)
    : label(_label), textureId(0)
{
    pathBuffer.fill('\0');
}

ImageSlot::~ImageSlot()
{
    ReleaseTexture();
}

const std::string& ImageSlot::GetLabel() const
{
    return label;
}

void ImageSlot::SetPath(const std::string& path)
{
    std::snprintf(pathBuffer.data(), pathBuffer.size(), "%s", path.c_str());
}

std::string ImageSlot::GetPathString() const
{
    return std::string(pathBuffer.data());
}

char* ImageSlot::GetPathBuffer()
{
    return pathBuffer.data();
}

const char* ImageSlot::GetPathBuffer() const
{
    return pathBuffer.data();
}

bool ImageSlot::LoadFromDisk()
{
    std::string path(pathBuffer.data());
    if(path.empty())
        return false;

    ImageReader reader;
    EnvMapImage loaded = reader.LoadImage(path);
    if(!loaded.IsValid())
    {
        std::cerr << "Failed to load image: " << pathBuffer.data() << std::endl;
        return false;
    }

    image = std::make_shared<EnvMapImage>(loaded);
    UploadTexture();
    return true;
}

void ImageSlot::Clear()
{
    image.reset();
    ReleaseTexture();
    pathBuffer.fill('\0');
}

bool ImageSlot::HasImage() const
{
    return (image != nullptr && image->IsValid());
}

int ImageSlot::GetWidth() const
{
    if(!HasImage())
        return 0;
    return image->GetWidth();
}

int ImageSlot::GetHeight() const
{
    if(!HasImage())
        return 0;
    return image->GetHeight();
}

const EnvMapImage* ImageSlot::GetImage() const
{
    if(!HasImage())
        return nullptr;
    return image.get();
}

GLuint ImageSlot::GetTextureId() const
{
    return textureId;
}

bool ImageSlot::Rotate90CW()
{
    if(!HasImage())
        return false;

    const int srcWidth = image->GetWidth();
    const int srcHeight = image->GetHeight();
    EnvMapImage rotated(srcHeight, srcWidth);

    for(int y = 0; y < srcHeight; ++y)
    {
        for(int x = 0; x < srcWidth; ++x)
        {
            unsigned int pixel = image->GetPixel(x, y);
            int newX = srcHeight - 1 - y;
            int newY = x;
            rotated.SetPixel(newX, newY, pixel);
        }
    }

    *image = rotated;
    UploadTexture();
    return true;
}

bool ImageSlot::Rotate90CCW()
{
    if(!HasImage())
        return false;

    const int srcWidth = image->GetWidth();
    const int srcHeight = image->GetHeight();
    EnvMapImage rotated(srcHeight, srcWidth);

    for(int y = 0; y < srcHeight; ++y)
    {
        for(int x = 0; x < srcWidth; ++x)
        {
            unsigned int pixel = image->GetPixel(x, y);
            int newX = y;
            int newY = srcWidth - 1 - x;
            rotated.SetPixel(newX, newY, pixel);
        }
    }

    *image = rotated;
    UploadTexture();
    return true;
}

bool ImageSlot::FlipHorizontal()
{
    if(!HasImage())
        return false;

    const int width = image->GetWidth();
    const int height = image->GetHeight();
    EnvMapImage flipped(width, height);

    for(int y = 0; y < height; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            unsigned int pixel = image->GetPixel(x, y);
            int newX = width - 1 - x;
            flipped.SetPixel(newX, y, pixel);
        }
    }

    *image = flipped;
    UploadTexture();
    return true;
}

bool ImageSlot::FlipVertical()
{
    if(!HasImage())
        return false;

    const int width = image->GetWidth();
    const int height = image->GetHeight();
    EnvMapImage flipped(width, height);

    for(int y = 0; y < height; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            unsigned int pixel = image->GetPixel(x, y);
            int newY = height - 1 - y;
            flipped.SetPixel(x, newY, pixel);
        }
    }

    *image = flipped;
    UploadTexture();
    return true;
}

void ImageSlot::UploadTexture()
{
    if(!HasImage())
        return;

    ReleaseTexture();

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    unsigned char* dataPtr = image->GetData().get();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image->GetWidth(), image->GetHeight(), 0,
        GL_RGBA, GL_UNSIGNED_BYTE, dataPtr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ImageSlot::ReleaseTexture()
{
    if(textureId != 0)
    {
        glDeleteTextures(1, &textureId);
        textureId = 0;
    }
}
