#pragma once

#include <map>
#include <string>
#include <vector>

#include <SDL.h>

class MediaManager
{
public:
    SDL_Texture *read(SDL_Renderer *renderer, const std::string &fname, int &w, int &h)
    {
        if (images.find(fname) == images.end())
        {
            const std::vector<std::string> candidatePaths = {
                fname,
                "./" + fname,
                "./resources/images/" + fname,
                "ALDTPA/ALDTPA/" + fname};

            SDL_Surface *character = nullptr;
            for (const std::string &path : candidatePaths)
            {
                character = SDL_LoadBMP(path.c_str());
                if (character != nullptr)
                {
                    break;
                }
            }

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
    std::map<std::string, SDL_Texture *> images;
};

inline MediaManager mm;
