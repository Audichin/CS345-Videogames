#pragma once

#include <SDL.h>
#include <iostream>

class Gamewindow
{
public:

    Gamewindow()
        : width(640), height(480), rect{0,0,50,50}, running(true)
    {
        std::cout << "[NOTICE]: Constructor called, initializing game window..." << std::endl;
    }

    ~Gamewindow()
    {
        std::cout << "[NOTICE]: Destructor called, cleaning SDL resources..." << std::endl;

        if(texture) SDL_DestroyTexture(texture);
        if(renderer) SDL_DestroyRenderer(renderer);
        if(window) SDL_DestroyWindow(window);
        if(surface) SDL_FreeSurface(surface);

        SDL_Quit();
    }

    /* ---------------- GETTERS ---------------- */

    int Get_width() const
    {
        return width;
    }

    int Get_height() const
    {
        return height;
    }

    SDL_Rect& Get_rect()
    {
        return rect;
    }

    bool Get_running() const
    {
        return running;
    }

    SDL_Window* Get_window()
    {
        return window;
    }

    SDL_Renderer* Get_renderer()
    {
        return renderer;
    }

    SDL_Surface* Get_surface()
    {
        return surface;
    }

    SDL_Texture* Get_texture()
    {
        return texture;
    }

    /* ---------------- SETTERS ---------------- */

    void Set_width(int width)
    {
        this->width = width;
    }

    void Set_height(int height)
    {
        this->height = height;
    }

    void Set_rect(SDL_Rect rect)
    {
        this->rect = rect;
    }

    void Set_running(bool running)
    {
        this->running = running;
    }

    void Set_window(SDL_Window* window)
    {
        this->window = window;
    }

    void Set_renderer(SDL_Renderer* renderer)
    {
        this->renderer = renderer;
    }

    void Set_surface(SDL_Surface* surface)
    {
        this->surface = surface;
    }

    void Set_texture(SDL_Texture* texture)
    {
        this->texture = texture;
    }

    /* ---------------- BOOT FUNCTION ---------------- */

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
            SDL_WINDOW_SHOWN
        );

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
            SDL_Quit();
            return 1;
        }

        surface = SDL_LoadBMP("character_main.bmp");

        if (!surface)
        {
            std::cerr << "[ERR]: SDL_LoadBMP failed: " << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }

        texture = SDL_CreateTextureFromSurface(renderer, surface);

        if (!texture)
        {
            std::cerr << "[ERR]: SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }

        SDL_FreeSurface(surface);
        surface = nullptr;

        return 0;
    }

private:

    int width;
    int height;

    SDL_Rect rect;

    bool running;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Surface* surface = nullptr;
    SDL_Texture* texture = nullptr;
};