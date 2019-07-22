#ifndef CONNECTOR_H
#define CONNECTOR_H

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<iostream>
#include<vector>
#include"scenes.h"

extern std::string gName;
extern int gWidth, gHeight;

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern std::vector<scene*> scenes;
extern int sceneId;
extern int time;
extern int pixelSize;

enum sceneTypes
{
    MENU,
    GAME
};

void init();
void windowInit();
SDL_Texture* loadTexture(const char *file);

#endif // CONNECTOR_H
