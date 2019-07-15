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


void windowInit()
{
    SDL_Init(SDL_INIT_VIDEO);
    gWindow = SDL_CreateWindow(gName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gWidth, gHeight,SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
}


void close()
{
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    init();
    windowInit();

    SDL_Event e;
    bool quit = false;

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
        scenes[sceneId]->draw();
    }

    close();
}
