#ifndef _WORKING_IMAGE_H_
#define _WORKING_IMAGE_H_

#include <utility>
#include <memory>

class EnvMapImage
{
public:
    EnvMapImage(int _width, int _height,  
        unsigned char* _data, size_t _dataSize);
    ~EnvMapImage();
    
    bool IsValid();

    int GetWidth();
    int GetHeight();
    size_t GetDataSize();

    unsigned int GetPixel(int x, int y);

private:
    int width;
    int height;
    std::shared_ptr<unsigned char> data;
    size_t dataSize;
};

#endif