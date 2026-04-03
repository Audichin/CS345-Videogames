#pragma once

#include <filesystem>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL.h>

class MediaManager
{
public:
    SDL_Texture *read(SDL_Renderer *renderer, const std::string &fname, int &w, int &h)
    {
        ensureAssetIndex();

        const std::filesystem::path requestedPath(fname);
        const std::string filename = requestedPath.filename().string();
        const std::string cacheKey = filename.empty() ? fname : filename;

        auto cached = images.find(cacheKey);
        if (cached != images.end())
        {
            SDL_QueryTexture(cached->second, nullptr, nullptr, &w, &h);
            return cached->second;
        }

        std::filesystem::path resolvedPath;
        if (std::filesystem::exists(requestedPath))
        {
            resolvedPath = std::filesystem::absolute(requestedPath);
        }
        else
        {
            auto asset = imagePaths.find(cacheKey);
            if (asset == imagePaths.end())
            {
                throw std::runtime_error("Could not find requested image in Assets folder: " + fname);
            }
            resolvedPath = asset->second;
        }

        const std::string normalizedPath = resolvedPath.make_preferred().string();

        SDL_Surface *character = SDL_LoadBMP(normalizedPath.c_str());
        if (character == nullptr)
        {
            throw std::runtime_error("Could not read .bmp file: " + normalizedPath);
        }

        SDL_SetColorKey(character, SDL_TRUE, SDL_MapRGB(character->format, 255, 0, 255));
        SDL_SetColorKey(character, SDL_TRUE, SDL_MapRGB(character->format, 173, 54, 58));

        SDL_Texture *charText = SDL_CreateTextureFromSurface(renderer, character);
        SDL_FreeSurface(character);

        if (charText == nullptr)
        {
            throw std::runtime_error("Failed to create texture from: " + normalizedPath);
        }

        images[cacheKey] = charText;

        SDL_Texture *result = charText;
        SDL_QueryTexture(result, nullptr, nullptr, &w, &h);
        return result;
    }

    ~MediaManager()
    {
        for (const auto &entry : images)
        {
            SDL_DestroyTexture(entry.second);
        }
    }

private:
    void ensureAssetIndex()
    {
        if (assetsIndexed)
        {
            return;
        }

        assetsIndexed = true;

        const std::vector<std::filesystem::path> assetRoots = {
            std::filesystem::path(__FILE__).parent_path().parent_path() / "Assets",
            std::filesystem::path("../Assets"),
            std::filesystem::path("./Assets"),
            std::filesystem::path("ALDTPA/Assets")};

        for (const auto &root : assetRoots)
        {
            if (!std::filesystem::exists(root) || !std::filesystem::is_directory(root))
            {
                continue;
            }

            for (const auto &entry : std::filesystem::recursive_directory_iterator(root))
            {
                if (!entry.is_regular_file())
                {
                    continue;
                }

                const auto extension = entry.path().extension().string();
                if (extension == ".bmp" || extension == ".BMP")
                {
                    imagePaths.try_emplace(
                        entry.path().filename().string(),
                        std::filesystem::absolute(entry.path()));
                }
            }
        }
    }

    bool assetsIndexed = false;
    std::map<std::string, SDL_Texture *> images;
    std::map<std::string, std::filesystem::path> imagePaths;
};

inline MediaManager mm;
