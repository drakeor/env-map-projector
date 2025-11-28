#include "FileDialog.h"

#include <filesystem>
#include <string>
#include <vector>

#include "portable-file-dialogs.h"

namespace
{
std::string ResolveInitialPath(const std::string& defaultPath)
{
    if(defaultPath.empty())
        return std::filesystem::current_path().string();

    std::filesystem::path path(defaultPath);
    std::error_code ec;
    if(std::filesystem::is_regular_file(path, ec))
        path = path.parent_path();

    if(path.empty())
        return defaultPath;

    return path.string();
}
}

std::string FileDialog::OpenImageFile(const std::string& defaultPath)
{
    static const std::vector<std::string> filters = {
        "Image Files", "*.png *.jpg *.jpeg *.hdr *.exr *.tga *.bmp",
        "All Files", "*"
    };

    auto selection = pfd::open_file("Select image", ResolveInitialPath(defaultPath), filters).result();
    if(selection.empty())
        return {};

    return selection.front();
}

std::string FileDialog::SelectDirectory(const std::string& defaultPath)
{
    auto folder = pfd::select_folder("Select directory", ResolveInitialPath(defaultPath)).result();
    return folder;
}
