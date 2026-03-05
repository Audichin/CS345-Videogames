#include <iostream>
#include <SDL.h>
#include <vector>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "phyphox.h"
#include "gamewindow.h"

void check_bounds(Gamewindow& game, int screen_width, int screen_height);
void center(Gamewindow& game, int screen_width, int screen_height);

int main(int argc, char* argv[])
{
    std::string ip;
    Phyphox::IMUData movement;

    Gamewindow game;

    if (game.boot() == 1)
    {
        return 1;
    }
    // LIST OF THINGS TO ADD HERE FOR FUTURE:
    // 1) Add more error handling
    // 2) Create an easier way to make screen size bigger and scale play to center space (for now)
    // 3) Add better image handling and when collision is made (for simple start to actual game)
    //  a) find a way to inculde BMP files from different folder so it isn't in main folder
    // 4) Move IP input to a dedicated pop up before game launches to avoid game 
    //  to not respond on boot if IP has not been entered yet

    std::cout << "[NOTICE]: Please make sure you are connected to the same wifi for your phone and PC" << std::endl;
    std::cout << "Enter Phyphox IP (Ex 192.168.1.152): ";
    std::cin >> ip;

    Phyphox poller(ip);
    std::cout << "Starting poll...\n";

    center(game, game.width, game.height);

    while (game.running) 
    { //constantly runs
        SDL_Event event; //creates variable to store key presses or similar player interaction
        while (SDL_PollEvent(&event) != 0)
        { 
            if (event.type == SDL_QUIT) 
            {
                game.running = false; //when quit event happens, stop the game
            }

        }
        SDL_RenderClear(game.renderer); //clears the renderer to be redrawn on the window
        SDL_RenderCopy(game.renderer, game.texture, NULL, &game.rect); //copies the texture to the renderer to be drawn on the window
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255); // colors the window black (0,0,0) no transparacy (255)


        // LIST OF THINGS TO ADD HERE FOR FUTURE:
        // 1) Add smoothness so movement isn't clunky
        // 2) If a data point is 0, just assume 0 change
        // 3) Find out how to use phone movement to change position
        // 4) Add a way to change sensitivity
        // 5) Maybe improve lag / responce times?
        // 6) Find out why it drifts when data is 0
        // 8) ***Fix deconstuctor running to segmentation fault upon exiting game***
        
        movement = poller.Phyphox_loop();
        
        if (movement.warn == true)
        {
            movement.ax = poller.Get_prevAcc();
            movement.gx = poller.Get_prevGyro();
        }
        else
        {
            game.rect.x += movement.ax; 
            game.rect.y += movement.ay;
        }

        check_bounds(game, game.width, game.height);

        std::cout << "X: " << game.rect.x << " | Y: " << game.rect.y << std::endl;

        // SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255); //sets color to red (255,0,0), no transparacy (255)
        // SDL_RenderFillRect(game.renderer, &game.rect); //creates rect using renderer and shape dimensions       

        SDL_RenderPresent(game.renderer); // update screen

        SDL_Delay(1000/60); //sets a frame limit of 60fps

        if (movement.err == 1)
        {
            game.running = false;
        }
    }

    game.terminate(game.window, game.renderer, game.texture, game.surface);

    return 0;
}

void check_bounds(Gamewindow& game, int screen_width, int screen_height)
{
        if (game.rect.x > screen_width) 
        {
            game.rect.x = 25;
        }
        if (game.rect.y > screen_height) 
        {
            game.rect.y = 25;
        } 
        if (game.rect.x < 0) 
        {
            game.rect.x = screen_width - 25;
        }
        if (game.rect.y < 0) 
        {
            game.rect.y = screen_height - 25;
        }
}

void center(Gamewindow& game, int screen_width, int screen_height)
{
    game.rect.x = (screen_width / 2) - (game.rect.w / 2);
    game.rect.y = (screen_height / 2) - (game.rect.h / 2);
}

