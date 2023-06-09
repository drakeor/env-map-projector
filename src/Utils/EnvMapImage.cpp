#include "EnvMapImage.h"

using namespace std;

EnvMapImage::EnvMapImage(int _width, int _height,
    unsigned char* _data, size_t _dataSize)
{
    width = _width;
    height = _height;
    data = std::shared_ptr<unsigned char>(_data);
    dataSize = _dataSize;
}

EnvMapImage::EnvMapImage(int _width, int _height)
{
    width = _width;
    height = _height;
    dataSize = _width * _height * 4;
    data = std::shared_ptr<unsigned char>(new unsigned char[dataSize]);
}

EnvMapImage::~EnvMapImage()
{

}

bool EnvMapImage::IsValid()
{
    return (data.get() != nullptr);
}

int EnvMapImage::GetWidth()
{
    return width;
}

int EnvMapImage::GetHeight()
{
    return height;
}

size_t EnvMapImage::GetDataSize()
{
    return dataSize;
}

unsigned int EnvMapImage::GetPixel(int x, int y)
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

std::shared_ptr<unsigned char> EnvMapImage::GetData()
{
    return data;
}