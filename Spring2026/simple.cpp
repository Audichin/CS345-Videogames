#include <SDL.h>
#include <iostream>
#include <vector>
#include <map>

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

class MediaManager{
    map<string,SDL_Texture *> images;
    public:
    SDL_Texture* read(SDL_Renderer *renderer,string fname,int &w,int &h){
        if (images.find(fname)==images.end()){
          SDL_Surface* character=SDL_LoadBMP(fname.c_str());
          if (character==NULL) throw "Could not read image.bmp file";
          SDL_Texture* charText=SDL_CreateTextureFromSurface(renderer,character);
          SDL_FreeSurface(character);
          if(charText==NULL) throw "Failed to create texture";
          images[fname]=charText;
        }
        SDL_Texture *result=images[fname];
        SDL_QueryTexture(result,NULL,NULL,&w,&h);
        return result;
    }
    ~MediaManager() {
        for (auto entry:images) SDL_DestroyTexture(entry.second);
    }
};
MediaManager mm;

class Character{
    SDL_Texture* charText;
    SDL_Rect src,dst; 
    float px,py,vx,vy,ax,ay;
  public:
  //  Character c=Character(renderer);
  //  Character b=c;
  //  int i=13;
  //  int b=i;
  //Character(const Character &other){
  //  other.px=10;
  //}  
  Character(SDL_Renderer *renderer){
    charText=mm.read(renderer,"image.bmp",src.w,src.h);
    src.x=0; src.y=0;
    dst.x=0; dst.y=0; dst.w=src.w; dst.h=src.h; 
    //px=rand()%640-32.0;
    //py=rand()%240-16.0;
    px=640/2-src.w/2;
    py=480/2-src.h/2;
    vx=rand()%10-5;
    vy=rand()%10-5;
   // cout << " px"<< px << " py" << py << endl;
    ax=0.0;
    ay=4.0;
  }
  void update(float dt){
 //   cout << "Update Called" << endl;
    vx=vx+ax*dt;
    px=px+vx*dt;
    vy=vy+ay*dt;
    py=py+vy*dt;
    if (px<0) px=640-32;
    if (px>640) px=0;
    if (py<0) py=480-32;
    if (py>480) py=0;        
    dst.x=px;
    dst.y=py;
//    cout << "Update" << dst.x << ",  "<< dst.y << endl;
  }
  void keyEvent(SDL_Keycode symbol){
    if (symbol==SDLK_SPACE) {
            vx=vx+10.0;
            vy=vy+-10.0;
        }
    if (symbol==SDLK_a) ax-=1.0;
    if (symbol==SDLK_d) ax+=1.0;
    if (symbol==SDLK_w) ay-=1.0;
    if (symbol==SDLK_s) ay+=1.0;
  }
  void draw(SDL_Renderer *renderer){
 //   cout << "Draw " << dst.x << ", "<< dst.y << endl;
    SDL_RenderCopy(renderer, charText, &src, &dst);
  }
};

int main(int argc, char* args[]) {
    cout << "Arg 0 " << args[0]<< endl;
   // cout << "Arg 1 " << args[1]<< endl;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    int retval=init(window,renderer,640,480);
    if (retval!=0) return retval;
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Gray color
    
    bool quit = false;

    SDL_Event e;
    int rcount=0;
    int bcount=0;
    try {
        cout << "Creating Square" << endl;
        //Character square(renderer);
        vector<Character*> characters;
        for (int i=0;i<2000;i++)
        characters.push_back(new Character(renderer));
        int last=SDL_GetTicks();
        while (!quit) {
            // double integrate accelerations to update velocity->position
            int now=SDL_GetTicks();
            float dt=(float)(now-last)/1000.0;
            last=now;
            for (auto c:characters)  c->update(dt);
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT)  quit = true;
                else if (e.type== SDL_KEYDOWN){
                    if (e.key.type== SDL_KEYDOWN) {
                        SDL_Keycode symbol=e.key.keysym.sym;
                        for(auto c:characters) c->keyEvent(symbol);
                    }
                }
            }
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, rcount, 0, bcount, 255); // Gray color
            for (auto c:characters) c->draw(renderer);
            SDL_RenderPresent(renderer);
            SDL_Delay(1000/240);
        }
        cleanup(window,renderer);
    }
    catch (char const* err) {
        cerr << err << endl; 
    }
    return 0;
}
