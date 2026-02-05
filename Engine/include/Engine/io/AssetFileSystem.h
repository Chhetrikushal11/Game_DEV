#pragma once
#include <filesystem>

namespace GAMEDEV_ENGINE
{
    class AssetFileSystem
    {
    public:
        // Get folder containing the executable
        std::filesystem::path GetExecutableFolder() const;

        // Get assets folder (dev or runtime)
        std::filesystem::path GetAssetsFolder() const;
    };
}