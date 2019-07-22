#ifndef SCENES_H
#define SCENES_H

#include"character.h"
#include"bullet.h"
#include"platform.h"

#include<unordered_set>


class scene
{
public:
    virtual void init() = 0;
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void handleEvent(SDL_Event) = 0;
    virtual void handleKeyState(const Uint8*) = 0;
    virtual void focus() = 0;
};

class game: public scene
{
public:
    point mouse;
    player pl;
    std::unordered_set<projectile*> bullets;
    std::unordered_set<projectile*> enemyBullets;
    std::unordered_set<character*> enemies;
    std::unordered_set<character*> dead;
    std::vector<platform> platforms;
    std::vector<platform> walls;

    SDL_Texture *background;
    SDL_Texture *healthbar;
    SDL_Texture *death_screen;
    Mix_Music *music;
    point offset;
    int floor_size;
    game();
    void clear();
    void init();
    void loadLevel();
    void draw();
    void update();
    void handleEvent(SDL_Event e);
    void handleKeyState(const Uint8* k);
    void focus();
};

class menu: public scene
{
public:
    menu();
    SDL_Texture *main_menu;
    SDL_Texture *controls;
    SDL_Texture *selector;
    Mix_Chunk *music;
    int channel;
    bool ctrl;
    bool new_game;
    std::vector<point> option_coord;
    point option_size;
    int curr_option;
    void init();
    void draw();
    void update();
    void handleEvent(SDL_Event e);
    void handleKeyState(const Uint8* k);
    void focus();
};

#endif // SCENES_H
