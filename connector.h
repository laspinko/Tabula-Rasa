#include<SDL.h>
#include<iostream>
#include<vector>
#include"scenes.h"

extern std::string gName;
extern int gWidth, gHeight;

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern std::vector<scene*> scenes;
extern int sceneId;

enum sceneTypes
{
    GAME
};

void init();
