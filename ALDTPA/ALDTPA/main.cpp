#include <./SDL.h>
#include <iostream>

using namespace std;

int error(string s){
    cerr << s << SDL_GetError()<<endl;
    return -1;
}

int init(SDL_Window* &window,SDL_Renderer* &renderer,int width,int height)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) 
    {
        return error("SDL could not initialize! SDL_Error: "); 
    }

    window = SDL_CreateWindow("ALDTPA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL) 
    {
        return error("Game window could not be created! SDL_Error:");
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) 
    {
        return error("Game Render could not be created! SDL_Error:");
    }

    return 0;
}

int down(int val1)
{
    int val2 = val1 * 2;
    val1 = val1 - val2
    return val1
}

class characters{
    public:
    Character
    {
        //add character
        //add character texture
        //add src
        //add dst
        //create update for adding change in movement & 
        //position data then with how they are printer
        //
    }
    private:
}

int main(int argc, char* args[]) 
{
    cout << "Arg 0 " << args[0]<< endl;
    cout << "Arg 1 " << args[1]<< endl;
    
    bool quit = false;
    Up = 0;
    Down = down(0);
    Left = 0;
    Right = 0;

    STL_Event e;
    SDL_Surface* character_main;
    SDL_Window* win = NULL;
    SDL_Renderer* ren = NULL;

    // initalise plane
    int ret = init(win,ren,640,480);
    if (ret != 0)
    {
        return ret;
    }

    //set background color
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);

    character_main = SDL_LoadBMP("Assets/character_main.bmp");
    if (character_main == NULL) 
    {
        return error("Could not read character_main.bmp file");
    }

    SDL_Texture* charText;
    character_main_Text = SDL_CreateTextureFromSurface(ren,character_main);
    if(charText == NULL)
    {
        return error("Failed to create texture");
    }

    // Main character inilalization data
    SDL_Rect src; src.x=0; src.y=0; src.w=64; src.h=64;
    SDL_Rect dst; dst.x=0; dst.y=0; dst.w=src.w; dst.h=src.h; 

    // ---- MAIN GAME LOOP ----
    while (!quit) 
    {
        while (SDL_PollEvent(&e) != 0) 
        {
            if (e.type == SDL_QUIT)
            {
                break;
            }





            
        }
    }
    
    // Clear the previous frame
    SDL_RenderClear(ren);

    // Update with next frame

    SDL_SetRenderDrawColor(ren, Dcount, 0, Acount, 255); // Gray color
    SDL_RenderCopy(ren, charText, &src, &dst);
    SDL_RenderPresent(ren);
    SDL_Delay(1000/240);

    return 0;
}
