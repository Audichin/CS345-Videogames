#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* argv[]){

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cerr <<"SDL_Init failed: " <<SDL_GetError() << std::endl;
        return 1;
    }
    std::cout << "SDL Initialized"<< std::endl;


    SDL_Window* window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window){
        std::cerr <<"SDL_CreateWindow failed" << SDL_GetError()<< std::endl;
        SDL_Quit();
        return 1;
    }


    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }


    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect rect = {100, 100, 200, 200};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);


    SDL_RenderPresent(renderer);

    SDL_Delay(1000/60);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}