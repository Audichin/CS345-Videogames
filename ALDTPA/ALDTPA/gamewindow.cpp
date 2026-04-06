#include <iostream>
#include <SDL.h>
#include <vector>

#include "gamewindow.hpp"
#include "phyphox.hpp"
#include "TextfileConverter.hpp"



int main(int argc, char *argv[])
{
    
    std::string ip;
    Phyphox::IMUData movement{};
    SDL_Event event;
    Gamewindow game;
    int clear_counter = 0;
    bool collide = false;
    std::vector<Block *> objectsList;
    int previousPlayerX = 0;
    int previousPlayerY = 0;


    if (game.boot() == 1)
    {
        return 1;
    }

    // LIST OF THINGS TO ADD HERE FOR FUTURE:
    // 1) Add more error handling
    // 2) Create an easier way to make screen size bigger and scale play to center space (for now)
    // 3) Add better image handling and when collision is made (for simple start to actual game)
    //    a) find a way to inculde BMP files from different folder so it isn't in main folder
    // 4) Move IP input to a dedicated pop up before game launches to avoid game
    //    to not respond on boot if IP has not been entered yet

    std::cout << "[NOTICE]: Please make sure you are connected to the same wifi for your phone and PC" << std::endl;
    std::cout << "Enter Phyphox IP (Ex 192.168.1.152): ";
    std::cin >> ip;

    Phyphox poller(ip);
    // Things to work on here:
    // 1) Need to check url to see if it is valid before booting the rest of the game
    // 2) Make it autostart if the phone connection is valid to avoid user hassle 
    std::cout << "Starting poll...\n";

    //can be moved anywhere, should only be ran to change current level
    //Only run once not in a loop
    //make sure to clear objects list before reuse
    try
    {
        TextToObjects(objectsList, game.Get_renderer() /*,"Filename.txt"*/);
    }
    catch (const std::exception &ex)
    {
        std::cerr << "[ERR]: " << ex.what() << std::endl;
        game.Set_running(false);
    }

    if (game.Get_player() != nullptr)
    {
        previousPlayerX = game.Get_player()->getX();
        previousPlayerY = game.Get_player()->getY();
    }

    while (game.Get_running())
    { // constantly runs
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                game.Set_running(false); // when quit event happens, stop the game
            }
            if (event.type == SDL_KEYDOWN && game.Get_player() != nullptr)
            {
                game.Get_player()->keyEvent(event.key.keysym.sym);
            }
        }

        // LIST OF THINGS TO ADD HERE FOR FUTURE:
        // 1) If a data point is 0, just assume 0 change
        // 2) Find out how to use phone movement to change position
        // 3) Add a way to change sensitivity
        movement = poller.Phyphox_loop();

        clear_counter++;
        if (clear_counter > 150)
        {
            // std::cout << "[NOTICE]: Resetting local Phyphox polling state..." << std::endl;
            poller.ClearBuffers();
            clear_counter = 0;
        }

        if (movement.warn != true && game.Get_player() != nullptr)
        {
            previousPlayerX = game.Get_player()->getX();
            previousPlayerY = game.Get_player()->getY();

            game.Get_player()->translate(
                static_cast<int>(movement.pitch),
                static_cast<int>(movement.yaw));

            game.Wrap_player_within_bounds();
        }

        // collision checks happen after movement so we can roll back to the last valid position
        collide = false;
        for (Block *object : objectsList)
        {
            if (object == nullptr || game.Get_player() == nullptr)
            {
                continue;
            }

            if (object->collided(game.Get_player()))
            {
                std::cout << "Collision detected with object at (" << object->getX() << ", " << object->getY() << ")" << std::endl;
                collide = true;
                game.Get_player()->setPosition(previousPlayerX, previousPlayerY);
                break;
            }
        }

        if (game.Get_player() != nullptr)
        {
            std::cout << "X: " << game.Get_player()->getX() << " | Y: " << game.Get_player()->getY() << std::endl;
        }

        game.Draw(objectsList);

        SDL_Delay(1000 / 60); // sets a frame limit of 60fps

        if (movement.err == 1)
        {
            game.Set_running(false);
        }
    }

    return 0;
}
