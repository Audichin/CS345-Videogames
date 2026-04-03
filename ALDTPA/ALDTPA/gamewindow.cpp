#include <iostream>
#include <SDL.h>
#include <vector>

#include "gamewindow.hpp"
#include "phyphox.hpp"




int main(int argc, char *argv[])
{
    
    std::string ip;
    Phyphox::IMUData movement{};
    SDL_Event event;
    Gamewindow game;
    int clear_counter = 0;
    static int frame = 0;
    float previousDirect = 0.0f; 
    bool collide=false;
    vector<Block> objectList;


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

        if (frame % 3 == 0)
        {
            movement = poller.Phyphox_loop();
        }

        frame++;
        clear_counter++;

        if (clear_counter > 600) // about 10 seconds at 60 FPS
        {
            poller.ClearBuffers();
            clear_counter = 0;
        }

        //loop for collision and object drawing
        for object in objectList{
            object.draw();
            if object.collide(game.Get_player()){collide = true;}
        }

        if (movement.warn == true || collide)
        {
            movement.direct = poller.Get_prevDirect();
            movement.yaw = poller.Get_prevYaw();
            movement.pitch = poller.Get_prevPitch();
            movement.roll = poller.Get_prevRoll();
        }
        else
        {
            game.Get_player()->translate(
                static_cast<int>((movement.pitch)),
                static_cast<int>(movement.yaw));
            previousDirect = movement.direct;
        }

        game.Wrap_player_within_bounds();

        if (game.Get_player() != nullptr)
        {
            std::cout << "X: " << game.Get_player()->getX() << " | Y: " << game.Get_player()->getY() << std::endl;
        }

        game.Draw();

        SDL_Delay(1000 / 60); // sets a frame limit of 60fps

        if (movement.err == 1)
        {
            game.Set_running(false);
        }
    }

    return 0;
}
