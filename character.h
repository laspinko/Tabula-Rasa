#ifndef CHARACTER_H
#define CHARACTER_H

#include"point.h"
#include"bullet.h"
#include<unordered_set>
#include<SDL.h>
#include<SDL_image.h>
class character
{
public:
    point pos;
    point vel;
    point face;
    point size;
    bool destroy;
    int maxhp;
    int hp;
    int shoot_cd;
    int damaged;
    int max_shoot_cd;
    int since_death;
    virtual void update(const character&, std::unordered_set<projectile*>&) = 0;
    virtual void getSprite(SDL_Texture*& , SDL_Rect&) = 0;
    virtual void getDeathSprite(SDL_Texture*& , SDL_Rect&) = 0;
    virtual void takeDmg(int) = 0;
};

class player: public character
{
public:
    bool on_ground;
    bool crouching;
    bool ignorePlatform;
    point hitbox;
    int jumping_left;
    int jump_ready;
    point last_ground;
    int swing;
    static SDL_Texture *texture;
    static SDL_Texture *texture_death;
    player();
    void update(const character&, std::unordered_set<projectile*>&);
    void getSprite(SDL_Texture*& , SDL_Rect&);
    void getDeathSprite(SDL_Texture*& , SDL_Rect&);
    void takeDmg(int);
};

class drone: public character
{
public:
    static SDL_Texture *texture;
    static SDL_Texture *texture_death;
    drone(point);
    void update(const character&, std::unordered_set<projectile*>&);
    void getSprite(SDL_Texture*& , SDL_Rect&);
    void getDeathSprite(SDL_Texture*& , SDL_Rect&);
    void takeDmg(int);
};

class turret: public character
{
public:
    static SDL_Texture *texture;
    static SDL_Texture *texture_death;
    turret(point);
    void update(const character&, std::unordered_set<projectile*>&);
    void getSprite(SDL_Texture*& , SDL_Rect&);
    void getDeathSprite(SDL_Texture*& , SDL_Rect&);
    void takeDmg(int);
};

class groundTurret: public character
{
public:
    static SDL_Texture *texture;
    static SDL_Texture *texture_death;
    groundTurret(point);
    void update(const character&, std::unordered_set<projectile*>&);
    void getSprite(SDL_Texture*& , SDL_Rect&);
    void getDeathSprite(SDL_Texture*& , SDL_Rect&);
    void takeDmg(int);
};

#endif // CHARACTER_H
