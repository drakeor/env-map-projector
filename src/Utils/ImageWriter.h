#ifndef _IMAGEWRITER_H_
#define _IMAGEWRITER_H_

#include "EnvMapImage.h"
#include <iostream>

class ImageWriter
{
public:
    ImageWriter();
    ~ImageWriter();

    void SaveImage(std::string fileName, EnvMapImage image);

private:

};

#endif