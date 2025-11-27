#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <iostream>

#include "ImageWriter.h"

ImageWriter::ImageWriter()
{

}

ImageWriter::~ImageWriter()
{
    
}

void ImageWriter::SaveImage(std::string fileName, EnvMapImage image)
{
    int width = image.GetWidth();
    int height = image.GetHeight();
    int channels = 4;
    int stride_length = 0;
    std::shared_ptr<unsigned char> data = image.GetData();

    int res = stbi_write_png(fileName.c_str(), width, height, 
        channels, data.get(), stride_length);

    //std::cout << "File write to " << fileName << " result: " << res << std::endl;
}