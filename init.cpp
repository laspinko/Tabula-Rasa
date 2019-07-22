#include"connector.h"
#include<iostream>

extern std::string gName;
extern int gWidth, gHeight;
extern std::vector<scene*> scenes;
extern int sceneId;


void init()
{
    gWidth = 640;
    gHeight = 400;
    gName = "Tabula Rasa";
    windowInit();
    scenes.push_back(new menu());
    scenes.push_back(new game());
    scenes[MENU]->focus();
}
