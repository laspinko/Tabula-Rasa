#include<iostream>
#include<SDL.h>
#include<vector>
#include"connector.h"

std::string gName;
int gWidth = 200, gHeight = 200;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

std::vector<scene*> scenes;
int sceneId = 0;

int time = 0;


SDL_Texture* loadTexture(const char *file)
{
    SDL_Surface *surface = IMG_Load(file);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void windowInit()
{
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);

    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 1024);

    gWindow = SDL_CreateWindow(gName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gWidth, gHeight,SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
}


void close()
{
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    IMG_Quit();
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    init();

    SDL_Event e;
    bool quit = false;

    const Uint8 *key_state;
    while(!quit)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                quit = true;
            }
            scenes[sceneId]->handleEvent(e);
        }
        key_state = SDL_GetKeyboardState(NULL);
        scenes[sceneId]->handleKeyState(key_state);
        scenes[sceneId]->update();
        scenes[sceneId]->draw();

        time ++;
        SDL_Delay(10);
    }

    close();
}
