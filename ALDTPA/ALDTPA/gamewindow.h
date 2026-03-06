#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>



class Gamewindow
{
    public:
        int width = 640;
        int height = 480;

        SDL_Window* window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN); //creates the window variable
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //creates a renderer to operate in the window
        SDL_Surface* surface = SDL_LoadBMP("character_main.bmp"); // loads an image to be used as a texture
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); // creates a texture from the surface to be rendered
        

        SDL_Rect rect = {0, 0, 50, 50}; // (pos.x, pos.y, len.x, len.y) creates shape of given size at given position
        bool running = true; //a bool for the game state

        int boot()
        {
            
            if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initalizes SDL and throws and error if it did not initalize
            { 
                std::cerr <<"[ERR]: SDL_Init failed: " <<SDL_GetError() << std::endl;
                return 1;
            }

            if (!window) // if the window doesn't exist throws an error and quits the application
            { 
                std::cerr <<"[ERR]: SDL_CreateWindow failed" << SDL_GetError()<< std::endl;
                SDL_Quit();
                return 1;
            }
            
            if (!renderer) // if renderer doesn't exist, closes the window and quits the application
            { 
                std::cerr <<"[ERR]: SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 1;
            }

            if (!surface) // if the surface doesn't exist, closes the window and renderer and quits the application
            { 
                std::cerr <<"[ERR]: SDL_LoadBMP failed: " << SDL_GetError() << std::endl;
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 1;
            }

            if (!texture) // if the texture doesn't exist, closes the window and renderer and surface and quits the application
            { 
                std::cerr <<"[ERR]: SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_FreeSurface(surface);
                SDL_Quit();
            }
            SDL_FreeSurface(surface);
            return 0;
        }

        void terminate(SDL_Window* win, SDL_Renderer* ren, SDL_Texture* tex, SDL_Surface* surf)
        {
            std::cout << "[NOTICE]: Terminating game window and all other resources..." << std::endl;
            SDL_DestroyRenderer(ren); //delete renderer obj
            SDL_DestroyWindow(win); //delete window obj
            SDL_DestroyTexture(tex); //delete texture obj
            SDL_FreeSurface(surf); //free surface obj
            SDL_Quit(); //quit SDL
        }

};