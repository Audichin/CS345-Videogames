#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>

#include <SDL.h>

#include "character.hpp"

class Gamewindow
{
public:
    Gamewindow()
        : width(640), height(480), running(true)
    {
        std::cout << "[NOTICE]: Constructor called, initializing game window..." << std::endl;
    }

    ~Gamewindow()
    {
        std::cout << "[NOTICE]: Destructor called, cleaning SDL resources..." << std::endl;

        player.reset();

        if (renderer)
        {
            SDL_DestroyRenderer(renderer);
        }
        if (window)
        {
            SDL_DestroyWindow(window);
        }

        SDL_Quit();
    }

    int Get_width() const
    {
        return width;
    }

    int Get_height() const
    {
        return height;
    }

    bool Get_running() const
    {
        return running;
    }

    SDL_Window *Get_window() const
    {
        return window;
    }

    SDL_Renderer *Get_renderer() const
    {
        return renderer;
    }

    Character *Get_player() const
    {
        return player.get();
    }

    void Set_width(int newWidth)
    {
        width = newWidth;
    }

    void Set_height(int newHeight)
    {
        height = newHeight;
    }

    void Set_running(bool isRunning)
    {
        running = isRunning;
    }

    int boot()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "[ERR]: SDL_Init failed: " << SDL_GetError() << std::endl;
            return 1;
        }

        window = SDL_CreateWindow(
            "Game Engine",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_SHOWN);

        if (!window)
        {
            std::cerr << "[ERR]: SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        if (!renderer)
        {
            std::cerr << "[ERR]: SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            window = nullptr;
            SDL_Quit();
            return 1;
        }

        try
        {
            player = std::make_unique<Character>(renderer, 1, "character_main", ".bmp");
        }
        catch (const char *message)
        {
            std::cerr << "[ERR]: " << message << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            renderer = nullptr;
            window = nullptr;
            SDL_Quit();
            return 1;
        }
        catch (const std::exception &ex)
        {
            std::cerr << "[ERR]: " << ex.what() << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            renderer = nullptr;
            window = nullptr;
            SDL_Quit();
            return 1;
        }

        Center_player();
        return 0;
    }

    void Draw()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (player)
        {
            player->draw(renderer);
        }

        SDL_RenderPresent(renderer);
    }

    void Center_player()
    {
        if (!player)
        {
            return;
        }

        player->setPosition(
            (width / 2) - (player->getWidth() / 2),
            (height / 2) - (player->getHeight() / 2));
    }

    void Wrap_player_within_bounds()
    {
        if (!player)
        {
            return;
        }

        int x = player->getX();
        int y = player->getY();

        if (x > width)
        {
            x = 25;
        }
        if (y > height)
        {
            y = 25;
        }
        if (x < 0)
        {
            x = width - player->getWidth();
        }
        if (y < 0)
        {
            y = height - player->getHeight();
        }

        player->setPosition(x, y);
    }

private:
    int width;
    int height;
    bool running;

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    std::unique_ptr<Character> player;
};
