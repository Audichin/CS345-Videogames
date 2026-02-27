#include <SDL.h>
#include <stdio.h>

// Screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[]) {
    fprintf(stdout,"Arg 0 %s\n",args[0]);
    fprintf(stdout,"Arg 1 %s\n",args[1]);
    // The window we'll be rendering to
    SDL_Window* window = NULL;
    // The renderer
    SDL_Renderer* renderer = NULL;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    } else { 
        // Create window
        window = SDL_CreateWindow("Simple SDL2 Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return -1;
        } else {
            // Create renderer for window
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL) {
                fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
                return -1;
            } else {
                // Set the renderer color to gray
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Gray color
                
                // Main loop flag
                int quit = 0;
                // Event handler
                SDL_Event e;
                int rcount=0;
                int bcount=0;

                // While application is running
                while (!quit) {
                    // Handle events on queue
                    while (SDL_PollEvent(&e) != 0) {
                        // User requests quit
                        if (e.type == SDL_QUIT) {
                            quit = 1;
                        }
                        if (e.type== SDL_KEYDOWN){
                            if (e.key.type== SDL_KEYDOWN) {
                              if (e.key.keysym.sym==SDLK_a) {
                                rcount++;
                                rcount=rcount%256;
                              }
                              if (e.key.keysym.sym==SDLK_d) {
                                rcount--;
                                if (rcount<0) rcount=255;
                              }
                            }
                        }
                    }

                    // Clear screen
                    SDL_RenderClear(renderer); 
                    SDL_SetRenderDrawColor(renderer, rcount, 0, bcount, 255); // Gray color

                    // Update screen
                    SDL_RenderPresent(renderer);
                    SDL_Delay(1000/240);
                }
            }
        }
    }

    // Destroy renderer 
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    
    // Destroy window
    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}
