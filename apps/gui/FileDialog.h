#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include <string>

class FileDialog
{
public:
    static std::string OpenImageFile(const std::string& defaultPath);
    static std::string SelectDirectory(const std::string& defaultPath);

private:
    static bool CommandExists(const char* command);
    static std::string RunCommand(const std::string& command);
    static std::string Trim(const std::string& value);
};

#endif
