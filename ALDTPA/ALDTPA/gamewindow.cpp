#include <iostream>
#include <SDL.h>
#include <vector>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "phyphox.h"
#include "gamewindow.h"

int main(int argc, char* argv[])
{
    std::string ip;
    Phyphox::IMUData movement;

    if (SDL_Init(SDL_INIT_VIDEO) < 0){ // Initalizes SDL and throws and error if it did not initalize
        std::cerr <<"SDL_Init failed: " <<SDL_GetError() << std::endl;
        return 1;
    }


    SDL_Window* window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, SDL_WINDOW_SHOWN); //creates the window variable
    if (!window){ // if the window doesn't exist throws an error and quits the application
        std::cerr <<"SDL_CreateWindow failed" << SDL_GetError()<< std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //creates a renderer to operate in the window
    if (!renderer) { // if renderer doesn't exist, closes the window and quits the application
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }


    std::cout << "[NOTICE]: Please make sure you are connected to the same wifi for your phone and PC" << std::endl;
    std::cout << "Enter Phyphox IP (Ex 192.168.1.152): ";
    std::cin >> ip;

    Phyphox poller(ip);
    std::cout << "Starting poll...\n";

    SDL_Rect rect = {500, 500, 50, 50}; // (pos.x, pos.y, len.x, len.y) creates shape of given size at given position
    SDL_Rect change_rect = rect;

    bool running = true; //a bool for the game state
    while (running) 
    { //constantly runs
        SDL_Event event; //creates variable to store key presses or similar player interaction
        while (SDL_PollEvent(&event) != 0)
        { 
            if (event.type == SDL_QUIT) 
            {
                running = false; //when quit event happens, stop the game
            }
        }

        movement = poller.Phyphox_loop(50);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // colors the window black (0,0,0) no transparacy (255)
        SDL_RenderClear(renderer); //clears the render color

        // LIST OF THINGS TO ADD HERE FOR FUTURE:
        // 1) Add smoothness so movement isn't clunky
        // 2) If a data point is 0, just assume 0 change
        // 3) Find out how to use phone movement to change position
        // 4) Add a way to change sensitivity
        // 5) Maybe improve lag / responce times?
        
        change_rect.x += movement.ax;
        change_rect.y += movement.ay;

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //sets color to red (255,0,0), no transparacy (255)
        SDL_RenderFillRect(renderer, &change_rect); //creates rect using renderer and shape dimensions
        

        SDL_RenderPresent(renderer); // update screen

        SDL_Delay(1000/60); //sets a frame limit of 60fps

    }

    //runs after game is not running
    SDL_DestroyRenderer(renderer); //delete renderer obj
    SDL_DestroyWindow(window); //delete window obj
    SDL_Quit(); //quit SQL

    return 0;
}