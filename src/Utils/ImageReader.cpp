#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

#include "ImageReader.h"
using namespace std;

ImageReader::ImageReader()
{

}

ImageReader::~ImageReader()
{

}

EnvMapImage ImageReader::LoadImage(string fileName)
{
    int width = 0;
    int height = 0;
    int channels = 0;

    // Although channels might be less than 4 (RGBA), we enforce RGBA output to make it easy
    // RGB images should just return a constant for the Alpha channel anyways...
    unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &channels, 4);
    size_t imageSize = width * height * 4;

    EnvMapImage envMapImage(width, height, imageData, imageSize);
    return envMapImage;
}