#include <iostream>
#include <SDL2/SDL.h>
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
    SDL_Event event;
    Gamewindow game;
    int clear_counter = 0;
    static int frame = 0;

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

    center(game, game.Get_width(), game.Get_height());

    while (game.Get_running()) 
    { //constantly runs
        while (SDL_PollEvent(&event) != 0)
        { 
            if (event.type == SDL_QUIT) 
            {
                game.Set_running(false); //when quit event happens, stop the game
            }
        }
        SDL_RenderClear(game.Get_renderer()); //clears the renderer to be redrawn on the window
        SDL_RenderCopy(game.Get_renderer(), game.Get_texture(), NULL, &game.Get_rect()); //copies the texture to the renderer to be drawn on the window
        SDL_SetRenderDrawColor(game.Get_renderer(), 0, 0, 0, 255); // colors the window black (0,0,0) no transparacy (255)

        // LIST OF THINGS TO ADD HERE FOR FUTURE:
        // 1) If a data point is 0, just assume 0 change
        // 2) Find out how to use phone movement to change position
        // 3) Add a way to change sensitivity

        if(frame % 3 == 0)
        {
            movement = poller.Phyphox_loop();
        }

        frame++;
        clear_counter++;

        if(clear_counter > 600)   // about 10 seconds at 60 FPS
        {
            poller.ClearBuffers();
            clear_counter = 0;
        }

        if (movement.warn == true)
        {
            movement.direct = poller.Get_prevDirect();
            movement.yaw = poller.Get_prevYaw();
            movement.pitch = poller.Get_prevPitch();
            movement.roll = poller.Get_prevRoll();
        } 
        else
        {
            game.Set_rect({game.Get_rect().x + static_cast<int>(movement.direct/3.50 - poller.Get_prevDirect()/3.50), 
            game.Get_rect().y + static_cast<int>(movement.pitch), game.Get_rect().w, game.Get_rect().h});
        }
  
        check_bounds(game, game.Get_width(), game.Get_height());

        std::cout << "X: " << game.Get_rect().x << " | Y: " << game.Get_rect().y << std::endl;

        // SDL_SetRenderDrawColor(game.Get_renderer(), 255, 0, 0, 255); //sets color to red (255,0,0), no transparacy (255)
        // SDL_RenderFillRect(game.Get_renderer(), &game.Get_rect()); //creates rect using renderer and shape dimensions       

        SDL_RenderPresent(game.Get_renderer()); // update screen

        SDL_Delay(1000/60); //sets a frame limit of 60fps

        if (movement.err == 1)
        {
            game.Set_running(false);
        }
    }

    return 0;
}

void check_bounds(Gamewindow& game, int screen_width, int screen_height)
{
        if (game.Get_rect().x > screen_width) 
        {
            game.Set_rect({25, game.Get_rect().y, game.Get_rect().w, game.Get_rect().h});
        }
        if (game.Get_rect().y > screen_height) 
        {
            game.Set_rect({game.Get_rect().x, 25, game.Get_rect().w, game.Get_rect().h});
        } 
        if (game.Get_rect().x < 0) 
        {
            game.Set_rect({screen_width - 25, game.Get_rect().y, game.Get_rect().w, game.Get_rect().h});
        }
        if (game.Get_rect().y < 0) 
        {
            game.Set_rect({game.Get_rect().x, screen_height - 25, game.Get_rect().w, game.Get_rect().h});
        }
}

void center(Gamewindow& game, int screen_width, int screen_height)
{
    game.Set_rect({(screen_width / 2) - (game.Get_rect().w / 2), \
    (screen_height / 2) - (game.Get_rect().h / 2), game.Get_rect().w, game.Get_rect().h});
}

