#ifndef _IMAGEREADER_H_
#define _IMAGEREADER_H_

#include "EnvMapImage.h"
#include <string>

class ImageReader
{
public:
    ImageReader();
    ~ImageReader();

    EnvMapImage LoadImage(std::string fileName);

private:
};

#endif