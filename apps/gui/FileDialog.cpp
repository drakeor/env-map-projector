#include "FileDialog.h"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <sstream>

namespace
{
std::string EscapePath(const std::string& path)
{
    std::ostringstream oss;
    for(char c : path)
    {
        if(c == '"')
            oss << "\\\"";
        else
            oss << c;
    }
    return oss.str();
}
}

std::string FileDialog::OpenImageFile(const std::string& defaultPath)
{
    std::string command;
    if(CommandExists("zenity"))
    {
        command = "zenity --file-selection --title=\"Select image\" --file-filter=\"Images | *.png *.jpg *.jpeg *.hdr *.exr *.tga\"";
        if(!defaultPath.empty())
        {
            command += " --filename=\"" + EscapePath(defaultPath) + "/\"";
        }
    }
    else if(CommandExists("kdialog"))
    {
        command = "kdialog --getopenfilename " + EscapePath(defaultPath.empty() ? std::string(".") : defaultPath) + " \"*.png *.jpg *.jpeg *.hdr *.exr *.tga\"";
    }
    else
    {
        return {};
    }

    return Trim(RunCommand(command));
}

std::string FileDialog::SelectDirectory(const std::string& defaultPath)
{
    std::string command;
    if(CommandExists("zenity"))
    {
        command = "zenity --file-selection --directory --title=\"Select directory\"";
        if(!defaultPath.empty())
        {
            command += " --filename=\"" + EscapePath(defaultPath) + "/\"";
        }
    }
    else if(CommandExists("kdialog"))
    {
        command = "kdialog --getexistingdirectory " + EscapePath(defaultPath.empty() ? std::string(".") : defaultPath);
    }
    else
    {
        return {};
    }

    return Trim(RunCommand(command));
}

bool FileDialog::CommandExists(const char* command)
{
    std::string checkCmd = "command -v ";
    checkCmd += command;
    checkCmd += " >/dev/null 2>&1";
    int result = std::system(checkCmd.c_str());
    return (result == 0);
}

std::string FileDialog::RunCommand(const std::string& command)
{
    std::string result;
    std::array<char, 1024> buffer;

    FILE* pipe = popen(command.c_str(), "r");
    if(!pipe)
        return result;

    while(fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr)
    {
        result += buffer.data();
    }

    int rc = pclose(pipe);
    (void)rc;
    return result;
}

std::string FileDialog::Trim(const std::string& value)
{
    size_t start = value.find_first_not_of("\r\n\t ");
    size_t end = value.find_last_not_of("\r\n\t ");
    if(start == std::string::npos)
        return {};
    return value.substr(start, end - start + 1);
}
