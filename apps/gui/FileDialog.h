#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include <string>

class FileDialog
{
public:
    static std::string OpenImageFile(const std::string& defaultPath);
    static std::string SelectDirectory(const std::string& defaultPath);
};

#endif
