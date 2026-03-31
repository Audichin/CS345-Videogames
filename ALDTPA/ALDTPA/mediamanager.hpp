#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include <SDL.h>

class MediaManager
{
public:
    SDL_Texture *read(SDL_Renderer *renderer, const std::string &fname, int &w, int &h)
    {
        ensureAssetIndex();

        if (images.find(fname) == images.end())
        {
            auto asset = imagePaths.find(fname);
            if (asset == imagePaths.end())
            {
                throw "Could not find requested image in Assets folder";
            }

            SDL_Surface *character = SDL_LoadBMP(asset->second.c_str());
            if (character == nullptr)
            {
                throw "Could not read image.bmp file";
            }

            SDL_SetColorKey(character, SDL_TRUE, SDL_MapRGB(character->format, 173, 54, 58));
            SDL_Texture *charText = SDL_CreateTextureFromSurface(renderer, character);
            SDL_FreeSurface(character);

            if (charText == nullptr)
            {
                throw "Failed to create texture";
            }

            images[fname] = charText;
        }

        SDL_Texture *result = images[fname];
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
                    imagePaths.try_emplace(entry.path().filename().string(), entry.path().string());
                }
            }
        }
    }

    bool assetsIndexed = false;
    std::map<std::string, SDL_Texture *> images;
    std::map<std::string, std::string> imagePaths;
};

inline MediaManager mm;
