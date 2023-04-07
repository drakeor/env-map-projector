#ifndef _WORKING_IMAGE_H_
#define _WORKING_IMAGE_H_

#include <utility>
#include <memory>

class EnvMapImage
{
public:
    EnvMapImage(int _width, int _height,  
        unsigned char* _data, size_t _dataSize);
    EnvMapImage(int _width, int _height);
    ~EnvMapImage();
    
    bool IsValid();

    int GetWidth();
    int GetHeight();
    size_t GetDataSize();

    unsigned int GetPixel(int x, int y);
    void SetPixel(int x, int y, unsigned int pixelValue);

    std::shared_ptr<unsigned char> GetData();

private:
    int width;
    int height;
    std::shared_ptr<unsigned char> data;
    size_t dataSize;
};

#endif