#include"character.h"
#include<iostream>
#include<cmath>
#include"connector.h"

///PLAYER

SDL_Texture* player::texture = NULL;
SDL_Texture* player::texture_death = NULL;

player::player()
{
    destroy = false;
    shoot_cd = 0;
    hp = maxhp = 100;
    shoot_cd = 0;
    jumping_left = 0;
    max_shoot_cd = 25;
    since_death = -1;
    damaged = 0;
    swing = 0;
    if(!texture)
    {
        texture = loadTexture("img\\player.png");
        texture_death = loadTexture("img\\player_death.png");
    }
}


void player::update(const character &pl, std::unordered_set<projectile*> &bullets)
{
    if(since_death != -1)
    {
        since_death ++;
        return;
    }

    if(damaged) damaged--;
    if(swing) swing --;
    if(shoot_cd) shoot_cd --;
}

void player::getSprite( SDL_Texture *&_texture, SDL_Rect &_rect)
{
    _texture = texture;
    if(swing > 0)
    {
        if(vel.y > 0)
        {
            _rect = {64 * 12, 0, 32, 30};
        } else {
            if(vel.x == 0)
            {
                _rect = {64 * 1, 0, 32, 30};
            } else {
                if((time / 10) % 2)
                {
                    _rect = {64 * 11, 0 , 32, 30};
                } else {
                    _rect = {64 * 1, 0, 32, 30};
                }
            }
        }
    } else {
        if(face.y == 0)
        {
            if(vel.y == 0)
            {
                if(vel.x == 0)
                {
                    _rect = {64 * 0, 0, 32, 30};
                } else {
                    if((time / 10) % 2)
                    {
                        _rect = {64 * 7,0,32,30};
                    } else {
                        _rect = {64 * 9,0,32,30};
                    }
                }
            } else {
                if(vel.y <= 0)
                {
                    _rect = {64 * 3, 0, 32, 30};
                } else {
                    _rect = {64 * 5, 0, 32, 30};
                }
            }
        } else {
            if(vel.y == 0)
            {
                if(vel.x == 0)
                {
                    _rect = {64 * 2, 0, 32, 30};
                } else {
                    if((time / 10) % 2)
                    {
                        _rect = {64 * 8,0,32,30};
                    } else {
                        _rect = {64 * 10,0,32,30};
                    }
                }
            } else {
                if(vel.y <= 0)
                {
                    _rect = {64 * 4, 0, 32, 30};
                } else {
                    _rect = {64 * 6, 0, 32, 30};
                }
            }
        }
    }
    if(face.x < 0)
    {
        _rect.y += 32;
    }
    if(damaged > 0)
    {
        _rect.x += 32;
    }
}

void player::getDeathSprite( SDL_Texture *&_texture, SDL_Rect &_rect)
{
    int frame = (since_death / 10) / 5;

    if(frame > 6)   frame = 5 + (frame - 5) % 2;
    _texture = texture_death;
    _rect = {0, frame * 32, 32, 30};
}

void player::takeDmg(int d)
{
    hp -= d;
    if(d > 0)   damaged = 15;
}


///DRONE
SDL_Texture* drone::texture = NULL;
SDL_Texture* drone::texture_death = NULL;
drone::drone(point _pos)
{
    damaged = 0;
    pos = _pos;
    destroy = false;
    shoot_cd = 0;
    size = point(40 * pixelSize,40 * pixelSize);
    hp = maxhp = 20;
    since_death = -1;
    max_shoot_cd = 150;
    if(!texture)
    {
        texture = loadTexture("img\\drone.png");
        texture_death = loadTexture("img\\drone_death.png");
    }
}

void drone::update(const character &pl, std::unordered_set<projectile*> &bullets)
{
    if(since_death != -1)
    {
        since_death ++;
        if(since_death >= 60)   destroy = true;
        return;
    }
    if(hp <= 0)
    {
        projectile *p = new heal();
        p->pos = pos;
        bullets.insert(p);

        since_death = 0;
        return;
    }

    vel = point(0,0);

    if(shoot_cd < max_shoot_cd - 50)
    {
        if(pl.pos.x < pos.x - 5)
        {
            vel = point(-1,0);
        }
        if(pl.pos.x > pos.x + 5)
        {
            vel = point(1,0);
        }
    }

    if(damaged) damaged --;

    pos += vel;

    if(pl.pos.x < pos.x)    face = point(-1, 0);
    else    face = point(1, 0);

    if(shoot_cd == 0)
    {
        if(pl.pos.y > pos.y)
        {
            projectile *p;
            p = new laser();
            p->pos = pos + point(0, size.y / 2);
            p->dir = point(0,1).normalize();
            bullets.insert(p);
            p = new laser();
            p->pos = pos + point(0, size.y / 2);
            p->dir = point(1,1).normalize();
            bullets.insert(p);
            p = new laser();
            p->pos = pos + point(0, size.y / 2);
            p->dir = point(-1,1).normalize();
            bullets.insert(p);
            shoot_cd = max_shoot_cd;
        }
    } else {
        shoot_cd --;
    }
}
void drone::getSprite( SDL_Texture *&_texture, SDL_Rect &_rect)
{
    _texture = texture;
    if(shoot_cd > max_shoot_cd - 20)
    {
        _rect = {40, 0, 40, 40};
    } else {
        _rect = {0, 0, 40, 40};
    }

    if(face.x > 0)
    {
        _rect.y += 40;
    }
    if(damaged > 0)
    {
        _rect.x = 80;
    }
}

void drone::getDeathSprite( SDL_Texture *&_texture, SDL_Rect &_rect)
{
    int frame = (since_death / 10) / 3;
    _texture = texture_death;
    _rect = {0, frame * 40, 40, 40};
}

void drone::takeDmg(int d)
{
    hp -= d;
    damaged = 10;
}


///TURRET
SDL_Texture* turret::texture = NULL;
SDL_Texture* turret::texture_death = NULL;
turret::turret(point _pos)
{
    damaged = 0;
    pos = _pos;
    destroy = false;
    shoot_cd = 0;
    size = point(35 * pixelSize,60 * pixelSize);
    hp = maxhp = 20;
    max_shoot_cd = 250;
    since_death = -1;
    if(!texture)
    {
        texture = loadTexture("img\\turret.png");
        texture_death = loadTexture("img\\turret_death.png");
    }
}

void turret::update(const character &pl, std::unordered_set<projectile*> &bullets)
{
    if(since_death != -1)
    {
        since_death ++;
        if(since_death >= 60)  destroy = true;
        return;
    }
    if(hp <= 0)
    {
        projectile *p = new heal();
        p->pos = pos;
        bullets.insert(p);

        since_death = 0;
        return;
    }

    if(damaged) damaged --;

    pos += vel;

    if(pl.pos.x < pos.x)    face = point(-1, 0);
    else    face = point(1, 0);

    if(shoot_cd == 0)
    {
        if(pl.pos.y > pos.y)
        {
            projectile *p = new rocket();
            p->pos = pos + point(0, size.y / 2);
            p->dir = (pl.pos - pos).normalize();
            p->speed = 2;
            bullets.insert(p);
            shoot_cd = max_shoot_cd;
        }
    } else {
        shoot_cd --;
    }
}
void turret::getSprite( SDL_Texture *&_texture, SDL_Rect &_rect)
{
    _texture = texture;

    _rect = {0,0,35,60};

    if(shoot_cd > max_shoot_cd - 20)
    {
        _rect = {35, 0, 35, 60};
    } else {
        _rect = {0, 0, 35, 60};
    }

    if(face.x > 0)
    {
        _rect.y += 60;
    }
    if(damaged > 0)
    {
        _rect.x = 70;
    }
}

void turret::getDeathSprite( SDL_Texture *&_texture, SDL_Rect &_rect)
{
    int frame = (since_death / 10) / 3;
    _texture = texture_death;
    _rect = {frame * 35, 0, 35, 60};
}

void turret::takeDmg(int d)
{
    hp -= d;
    damaged = 10;
}

///GROUND TURRET
SDL_Texture* groundTurret::texture = NULL;
SDL_Texture* groundTurret::texture_death = NULL;
groundTurret::groundTurret(point _pos)
{
    damaged = 0;
    pos = _pos;
    destroy = false;
    shoot_cd = 0;
    max_shoot_cd = 250;
    since_death = -1;
    size = point(32 * pixelSize,48 * pixelSize);
    hp = maxhp = 30;
    if(!texture)
    {
        texture = loadTexture("img\\groundEnemy.png");
        texture_death = loadTexture("img\\groundEnemy_death.png");
    }
}

void groundTurret::update(const character &pl, std::unordered_set<projectile*> &bullets)
{
    if(since_death != -1)
    {
        since_death ++;
        if(since_death >= 60)  destroy = true;
        return;
    }
    if(hp <= 0)
    {
        projectile *p = new heal();
        p->pos = pos;
        p->speed = 0;
        bullets.insert(p);

        since_death = 0;
        return;
    }

    if(damaged) damaged --;

    pos += vel;

    if(pl.pos.x < pos.x)    face = point(-1, 0);
    else    face = point(1, 0);

    if(shoot_cd == 0)
    {
        projectile *p = new bomb();
        p->pos = pos - point(0, size.y / 2);
        p->speed = 2 * pixelSize;

        double g = -0.02 * pixelSize; /// from bomb::update
        double x = pl.pos.x - pos.x;
        double y = pl.pos.y - pos.y;
        double eff = 2 * p->speed * p->speed / g;
        double rootterm = eff*(eff - 2*y) - 2*x*x;
        if(rootterm < 0) {
            p->dir = point((x>0?1:-1),-1).normalize();
        } else {
            double dy = (eff - sqrt(rootterm))/2;
            double ang = SDL_atan2(dy, x);
            p->dir = point(SDL_cos(ang), SDL_sin(ang));
        }

        bullets.insert(p);
        shoot_cd = max_shoot_cd;
    } else {
        shoot_cd --;
    }
}
void groundTurret::getSprite( SDL_Texture *&_texture, SDL_Rect &_rect)
{
    _texture = texture;
    if(shoot_cd > max_shoot_cd - 20)
    {
        _rect = {32, 0, 32, 48};
    } else {
        _rect = {0, 0, 32, 48};
    }

    if(face.x > 0)
    {
        _rect.y += 48;
    }
    if(damaged > 0)
    {
        _rect.x = 64;
    }
}


void groundTurret::getDeathSprite( SDL_Texture *&_texture, SDL_Rect &_rect)
{
    int frame = (since_death / 10) / 3;
    _texture = texture_death;
    _rect = {frame * 32, 0, 32, 48};
}

void groundTurret::takeDmg(int d)
{
    hp -= d;
    damaged = 10;
}
