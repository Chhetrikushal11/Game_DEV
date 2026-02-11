#pragma once
#include <filesystem>
#include <vector>

namespace GAMEDEV_ENGINE
{
    class AssetFileSystem
    {
    public:
        // Get folder containing the executable
        std::filesystem::path GetExecutableFolder() const;

        // Get assets folder (dev or runtime)
        std::filesystem::path GetAssetsFolder() const;

        // to load glsl
        std::vector<char> LoadFile(const std::filesystem::path& path);
        std::vector<char> LoadAssetFile(const std::string& relativepath);
        std::string LoadAssetFileText(const std::string& relativepath);
    };
}