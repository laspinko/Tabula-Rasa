#include"connector.h"
#include<iostream>

extern std::string gName;
extern int gWidth, gHeight;
extern std::vector<scene*> scenes;
extern int sceneId;


void init()
{
    gWidth = 500;
    gHeight = 500;
    gName = "Sample";
    scenes.push_back(new game());
    sceneId = GAME;
}
