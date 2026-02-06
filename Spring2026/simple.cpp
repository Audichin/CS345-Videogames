#include <SDL.h>
#include <iostream>

using namespace std;

int error(string s){
    cerr << s << SDL_GetError()<<endl;
    return -1;
}

int init(SDL_Window* &window,SDL_Renderer* &renderer,int width,int height){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return error("SDL could not initialize! SDL_Error: "); 
    window = SDL_CreateWindow("Simple SDL2 Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL) return error("Window could not be created! SDL_Error:");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) return error("Renderer could not be created! SDL_Error:");
    return 0;
}

void cleanup(SDL_Window* &window,SDL_Renderer* &renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* args[]) {
    cout << "Arg 0 " << args[0]<< endl;
    cout << "Arg 1 " << args[1]<< endl;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    int retval=init(window,renderer,640,480);
    if (retval!=0) return retval;
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Gray color
    
    bool quit = false;

    SDL_Event e;
    int rcount=0;
    int bcount=0;
    SDL_Surface* character;
    character=SDL_LoadBMP("image.bmp");
    if (character==NULL) return error("Could not read image.bmp file");
    SDL_Texture* charText;
    charText=SDL_CreateTextureFromSurface(renderer,character);
    if(charText==NULL) return error("Failed to create texture");
    SDL_Rect src; src.x=0; src.y=0; src.w=32; src.h=32;
    SDL_Rect dst; dst.x=0; dst.y=0; dst.w=src.w; dst.h=src.h; 
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT)  quit = true;
            else if (e.type== SDL_KEYDOWN){
                if (e.key.type== SDL_KEYDOWN) {
                    SDL_Keycode symbol=e.key.keysym.sym;
                    if (symbol==SDLK_a) dst.x--;
                    if (symbol==SDLK_d) dst.x++;
                    if (symbol==SDLK_w) dst.y--;
                    if (symbol==SDLK_s) dst.y++;
                }
            }
        }
        if (dst.x<0) dst.x=640-32;
        if (dst.x>640) dst.x=0;
        if (dst.y<0) dst.y=480-32;
        if (dst.y>480) dst.y=0;

        SDL_RenderClear(renderer);
        //Update Screen
        SDL_SetRenderDrawColor(renderer, rcount, 0, bcount, 255); // Gray color
        SDL_RenderCopy(renderer, charText, &src, &dst);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/240);
    }
    cleanup(window,renderer);
    return 0;
}
