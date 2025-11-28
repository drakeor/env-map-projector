#include "EnvMapImage.h"

#include <algorithm>
#include <cstring>

using namespace std;

EnvMapImage::EnvMapImage(int _width, int _height,
    unsigned char* _data, size_t _dataSize)
{
    AllocateStorage(_width, _height);
    size_t copySize = dataSize;
    if(_dataSize > 0)
        copySize = std::min(copySize, _dataSize);
    CopyFrom(_data, copySize);
}

EnvMapImage::EnvMapImage(int _width, int _height)
{
    AllocateStorage(_width, _height);
    if(data)
    {
        std::memset(data.get(), 0, dataSize);
    }
}

EnvMapImage::~EnvMapImage()
{

}

bool EnvMapImage::IsValid() const
{
    return (data.get() != nullptr);
}

int EnvMapImage::GetWidth() const
{
    return width;
}

int EnvMapImage::GetHeight() const
{
    return height;
}

size_t EnvMapImage::GetDataSize() const
{
    return dataSize;
}

unsigned int EnvMapImage::GetPixel(int x, int y) const
{
    unsigned int pixel = 0;

    // Sanity checking
    if(data.get() == nullptr)
        return pixel;

    // Bounds checking
    if(x < 0 || y < 0 || x >= width || y >= height)
        return pixel;

    // Grab the pixel
    int index = y * width * 4 + x * 4;
    pixel = 
        (data.get()[index] << 24)
        | (data.get()[index+1] << 16)
        | (data.get()[index+2] << 8)
        | (data.get()[index+3] << 0);

    return pixel;
}

void EnvMapImage::SetPixel(int x, int y, unsigned int pixelValue)
{
    // Sanity checking
    if(data.get() == nullptr)
        return;

    // Bounds checking
    if(x < 0 || y < 0 || x >= width || y >= height)
        return;

    // Set the image data appropiately
    int index = y * width * 4 + x * 4;
    data.get()[index]   = (pixelValue >> 24) & 0xFF;
    data.get()[index+1] = (pixelValue >> 16) & 0xFF;
    data.get()[index+2] = (pixelValue >> 8) & 0xFF;
    data.get()[index+3] = pixelValue & 0xFF;
}

std::shared_ptr<unsigned char> EnvMapImage::GetData() const
{
    return data;
}

void EnvMapImage::AllocateStorage(int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
    if(width <= 0 || height <= 0)
    {
        dataSize = 0;
        data.reset();
        return;
    }
    dataSize = static_cast<size_t>(width) * static_cast<size_t>(height) * 4u;
    data = std::shared_ptr<unsigned char>(new unsigned char[dataSize], std::default_delete<unsigned char[]>());
}

void EnvMapImage::CopyFrom(const unsigned char* src, size_t byteCount)
{
    if(!data || byteCount == 0 || src == nullptr)
        return;
    std::memcpy(data.get(), src, byteCount);
    if(byteCount < dataSize)
    {
        std::memset(data.get() + byteCount, 0, dataSize - byteCount);
    }
}