#pragma once
#include <SDL2/SDL.h>

class Gamewindow
{
    public:
        int width = 640;
        int height = 480;

        SDL_Window* window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN); //creates the window variable
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //creates a renderer to operate in the window


        SDL_Rect rect = {300, 100, 50, 50}; // (pos.x, pos.y, len.x, len.y) creates shape of given size at given position
        bool running = true; //a bool for the game state


        int boot()
        {
            
            if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initalizes SDL and throws and error if it did not initalize
            { 
            std::cerr <<"SDL_Init failed: " <<SDL_GetError() << std::endl;
            return 1;
            }

            if (!window) // if the window doesn't exist throws an error and quits the application
            { 
                std::cerr <<"SDL_CreateWindow failed" << SDL_GetError()<< std::endl;
                SDL_Quit();
                return 1;
            }

            
            if (!renderer) // if renderer doesn't exist, closes the window and quits the application
            { 
                std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 1;
            }
        }
        void terminate(SDL_Window* win, SDL_Renderer* ren)
        {
            SDL_DestroyRenderer(ren); //delete renderer obj
            SDL_DestroyWindow(win); //delete window obj
            SDL_Quit(); //quit SQL
        }
};