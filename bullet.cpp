#include "bullet.h"
#include<iostream>
#include"connector.h"


///BULLET

SDL_Texture* bullet::texture = NULL;

bullet::bullet()
{
    age = 0;
    destroy = false;
    speed = 2;
    size = point(2 * pixelSize, 2 * pixelSize);
    dmg = 10;
    if(!texture)
    {
        texture = loadTexture("img\\bullet.png");
    }
}

void bullet::update(character& player)
{
    pos += dir * speed;
    age ++;
    if(age > 200)   destroy = true;
}

bool bullet::colision(point from, point sz)
{
    if(from.x - sz.x / 2 - size.x / 2 < pos.x && pos.x < from.x + sz.x / 2 + size.x / 2 &&
       from.y - sz.y / 2 - size.y / 2 < pos.y && pos.y < from.y + sz.y / 2 + size.y)
    {
        return true;
    }
    return false;
}

void bullet::getSprite( SDL_Texture *&_texture, SDL_Rect &_rect)
{
    _texture = texture;
    if((time / 10) % 2)
    {
        _rect = {0,0,2,2};
    } else {
        _rect = {2,0,2,2};
    }
}


///LASER

SDL_Texture* laser::texture = NULL;

laser::laser()
{
    age = 0;
    destroy = false;
    speed = 2;
    size = point(5 * pixelSize, 5 * pixelSize);
    dmg = 10;
    if(!texture)
    {
        texture = loadTexture("img\\laser.png");
    }
}

void laser::update(character& player)
{
    pos += dir * speed;
    age ++;
    if(age > 200)   destroy = true;
}

bool laser::colision(point from, point sz)
{
    if(from.x - sz.x / 2 - size.x / 2 < pos.x && pos.x < from.x + sz.x / 2 + size.x / 2 &&
       from.y - sz.y / 2 - size.y / 2 < pos.y && pos.y < from.y + sz.y / 2 + size.y)
    {
        return true;
    }
    return false;
}

void laser::getSprite( SDL_Texture *&_texture, SDL_Rect &_rect)
{
    _texture = texture;
    if(dir.x == 0)
    {
        _rect = {0,0,5,5};
    }
    if(dir.x < 0)
    {
        _rect = {5,0,5,5};
    }
    if(dir.x > 0)
    {
        _rect = {10,0,5,5};
    }

}
///ROCKET

SDL_Texture* rocket::texture = NULL;

rocket::rocket()
{
    age = 0;
    destroy = false;
    speed = 5;
    size = point(7 * pixelSize, 7 * pixelSize);
    dmg = 10;
    if(!texture)
    {
        texture = loadTexture("img\\rocket.png");
    }
}

void rocket::update(character& player)
{
    const double pi = SDL_acos(-1);
    double ang = SDL_atan2(dir.y, dir.x);
    point to_pl = player.pos - pos;
    double ang_step = 0.02;
    if(to_pl.x * dir.y - to_pl.y * dir.x < 0)
    {
        ang += ang_step;
    } else {
        ang -= ang_step;
    }

    dir = point(SDL_cos(ang), SDL_sin(ang));

    pos += dir * speed;
    age ++;
    if(age > 500)   destroy = true;
}

bool rocket::colision(point from, point sz)
{
    if(from.x - sz.x / 2 - size.x / 2 < pos.x && pos.x < from.x + sz.x / 2 + size.x / 2 &&
       from.y - sz.y / 2 - size.y / 2 < pos.y && pos.y < from.y + sz.y / 2 + size.y)
    {
        return true;
    }
    return false;
}

void rocket::getSprite( SDL_Texture *&_texture, SDL_Rect &_rect)
{
    _texture = texture;
    const double pi = SDL_acos(-1);
    double ang = SDL_atan2(-dir.y, dir.x);
    while(ang < 0)  ang += 2 * pi;

    if(pi * 1 / 8 <= ang && ang < pi * 3 / 8) _rect = {14, 0, 7, 7};
    else if(pi * 3 / 8 <= ang && ang < pi * 5 / 8)  _rect = {7, 0, 7, 7};
    else if(pi * 5 / 8 <= ang && ang < pi * 7 / 8)  _rect = {0, 0, 7, 7};
    else if(pi * 7 / 8 <= ang && ang < pi * 9 / 8)  _rect = {0, 7, 7, 7};
    else if(pi * 9 / 8 <= ang && ang < pi * 11 / 8)  _rect = {0, 14, 7, 7};
    else if(pi * 11 / 8 <= ang && ang < pi * 13 / 8)  _rect = {7, 14, 7, 7};
    else if(pi * 13 / 8 <= ang && ang < pi * 15 / 8)  _rect = {14, 14, 7, 7};
    else    _rect = {14, 7, 7, 7};
}

///HEAL

SDL_Texture* heal::texture = NULL;

heal::heal()
{
    age = 0;
    destroy = false;
    speed = 0.5 * pixelSize;
    size = point(15 * pixelSize, 15 * pixelSize);
    dmg = -15;
    if(!texture)
    {
        texture = loadTexture("img\\health.png");
    }
    dir = point(0, 1);
}

void heal::update(character& player)
{
    pos += dir * speed;

    age ++;
    if(age > 500)   destroy = true;
}

bool heal::colision(point from, point sz)
{
    if(from.x - sz.x / 2 - size.x / 2 < pos.x && pos.x < from.x + sz.x / 2 + size.x / 2 &&
       from.y - sz.y / 2 - size.y / 2 < pos.y && pos.y < from.y + sz.y / 2 + size.y)
    {
        return true;
    }
    return false;
}

void heal::getSprite( SDL_Texture *&_texture, SDL_Rect &_rect)
{
    _texture = texture;
    switch((time / 10) % 4)
    {
    case 0:
        _rect = {0,0,7,7};
        break;
    case 1:
        _rect = {7,0,7,7};
        break;
    case 2:
        _rect = {14,0,7,7};
        break;
    case 3:
        _rect = {21,0,7,7};
        break;
    }
}

///BOMB

SDL_Texture* bomb::texture = NULL;

bomb::bomb()
{
    age = 0;
    destroy = false;
    size = point(6 * pixelSize, 6 * pixelSize);
    dmg = 10;
    if(!texture)
    {
        texture = loadTexture("img\\bomb.png");
    }
}

void bomb::update(character& player)
{

    dir = (dir * speed + point(0,0.02 * pixelSize));
    speed = SDL_sqrt(dir.x * dir.x + dir.y * dir.y);
    dir = dir.normalize();

    pos += dir * speed;
    age ++;
    if(age > 500)   destroy = true;
}

bool bomb::colision(point from, point sz)
{
    if(from.x - sz.x / 2 - size.x / 2 < pos.x && pos.x < from.x + sz.x / 2 + size.x / 2 &&
       from.y - sz.y / 2 - size.y / 2 < pos.y && pos.y < from.y + sz.y / 2 + size.y)
    {
        return true;
    }
    return false;
}

void bomb::getSprite( SDL_Texture *&_texture, SDL_Rect &_rect)
{
    _texture = texture;
    _rect = {0, 0, 6, 6};
}

///SWING

SDL_Texture* swing::texture = NULL;

swing::swing()
{
    age = 0;
    destroy = false;
    size = point(20 * pixelSize, 10 * pixelSize);
    dmg = 15;
    if(!texture)
    {
        texture = loadTexture("img\\swing.png");
    }
}

void swing::update(character& player)
{
    age ++;
    if(age > 5)   destroy = true;
}

bool swing::colision(point from, point sz)
{
    if(age < 5) return false;
    if(from.x - sz.x / 2 - size.x / 2 < pos.x && pos.x < from.x + sz.x / 2 + size.x / 2 &&
       from.y - sz.y / 2 - size.y / 2 < pos.y && pos.y < from.y + sz.y / 2 + size.y)
    {
        return true;
    }
    return false;
}

void swing::getSprite( SDL_Texture *&_texture, SDL_Rect &_rect)
{
    _texture = texture;
    if(dir.x > 0)
    {
        _rect = {0, 0, 20, 10};
    } else {
        _rect = {0, 10, 20, 10};
    }
}
