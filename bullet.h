#ifndef BULLET_H
#define BULLET_H

#include"point.h"
#include<SDL.h>
#include<SDL_image.h>

class character;

class projectile
{
public:
    point pos;
    point size;
    point dir;
    double speed;
    int age;
    bool destroy;
    double dmg;
    virtual void update(character&) = 0;
    virtual bool colision(point, point) = 0;
    virtual void getSprite( SDL_Texture*& texture, SDL_Rect &rect) = 0;
};

class bullet: public projectile
{
public:
    static SDL_Texture *texture;
    bullet();
    void update(character&);
    bool colision(point, point);
    void getSprite( SDL_Texture*& texture, SDL_Rect &rect);
};

class laser: public projectile
{
public:
    static SDL_Texture *texture;
    laser();
    void update(character&);
    bool colision(point, point);
    void getSprite( SDL_Texture*& texture, SDL_Rect &rect);
};

class rocket: public projectile
{
public:
    static SDL_Texture *texture;
    rocket();
    void update(character&);
    bool colision(point, point);
    void getSprite( SDL_Texture*& texture, SDL_Rect &rect);
};

class heal: public projectile
{
public:
    static SDL_Texture *texture;
    heal();
    void update(character&);
    bool colision(point, point);
    void getSprite( SDL_Texture*& texture, SDL_Rect &rect);
};

class bomb: public projectile
{
public:
    static SDL_Texture *texture;
    bomb();
    void update(character&);
    bool colision(point, point);
    void getSprite( SDL_Texture*& texture, SDL_Rect &rect);
};

class swing: public projectile
{
public:

    static SDL_Texture *texture;
    swing();
    void update(character&);
    bool colision(point, point);
    void getSprite( SDL_Texture*& texture, SDL_Rect &rect);
};

#endif // BULLET_H
