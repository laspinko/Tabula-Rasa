#include"connector.h"
#include"point.h"


int pixelSize = 2;

game::game()
{
    background = loadTexture("img\\background.png");
    healthbar = loadTexture("img\\healthbar.png");
    death_screen = loadTexture("img\\press_esc.png");
    music = Mix_LoadMUS("music\\nz.wav");
    init();
}



void game::clear()
{
    for(auto i: enemies)
    {
        delete i;
    }
    enemies.clear();

    for(auto i: enemyBullets)
    {
        delete i;
    }
    enemyBullets.clear();

    for(auto i: bullets)
    {
        delete i;
    }
    bullets.clear();

    for(auto i: dead)
    {
        ///TODO
    }
    dead.clear();
    Mix_HaltMusic();
}

void game::init()
{
    clear();

    //Mix_PlayMusic(music, -1);
    //Mix_PauseMusic();

    floor_size = 7 * pixelSize;

    offset = point(0, 1000 * pixelSize - gHeight);

    pl = player();

    pl.hitbox = point(24 * pixelSize,30 * pixelSize);
    pl.size = point(32 * pixelSize,30 * pixelSize);
    pl.pos = point(25 * pixelSize ,938 * pixelSize - pl.hitbox.y / 2);
    pl.vel = point(0,0);
    pl.face = point(1, 0);

    loadLevel();
}

void game::loadLevel()
{

    SDL_Surface *m = IMG_Load("img//map.png");

    Uint8 r, g, b;
    int w = m->w, h = m->h;
    Uint32 *p = (Uint32*)m->pixels;

    enum types{
        VOID,
        WALL,
        PLATFORM,
        GROUNDTURRET,
        FLYINGTURRET,
        DRONE
    };
    short** pix = new short*[w];

    for(int x = 0; x < w; x ++)
    {
        pix[x] = new short[h];
        for(int y = 0; y < h; y ++)
        {
            SDL_GetRGB(p[x+y*w], m->format, &r, &g, &b);
            pix[x][y] = VOID;
            if(r == 0xff && g == 0xff && b == 0xff) pix[x][y] = WALL;
            if(r == 0x00 && g == 0x00 && b == 0xff) pix[x][y] = PLATFORM;
            if(r == 0x00 && g == 0xff && b == 0x00) pix[x][y] = GROUNDTURRET;
            if(r == 0xff && g == 0x00 && b == 0x00) pix[x][y] = FLYINGTURRET;
            if(r == 0xff && g == 0xff && b == 0x00) pix[x][y] = DRONE;
        }
    }

    for(int x = 0; x < w; x ++)
    {
        for(int y = 0; y < h; y ++)
        {
            if(pix[x][y] == WALL && (x == 0 || pix[x - 1][y] != WALL))
            {
                int sz = 0;
                for(int i = x; i < w && pix[i][y] == WALL; i ++)    sz ++;
                if(sz > 1)
                {
                    walls.push_back(platform(point(x * pixelSize, y * pixelSize), sz * pixelSize, false));
                }
            }
            if(pix[x][y] == WALL && (y == 0 || pix[x][y - 1] != WALL))
            {
                int sz = 0;
                for(int i = y; i < h && pix[x][i] == WALL; i ++)    sz ++;
                if(sz > 1)
                {
                    walls.push_back(platform(point(x * pixelSize, y * pixelSize), sz * pixelSize, true, false));
                    walls.push_back(platform(point(x * pixelSize, y * pixelSize), sz * pixelSize, true, true));
                }
            }
            if(pix[x][y] == PLATFORM && (x == 0 || pix[x - 1][y] != PLATFORM))
            {
                platforms.push_back(platform(point(x * pixelSize, y * pixelSize), 100 * pixelSize));
            }
            if(pix[x][y] == GROUNDTURRET)
            {
                enemies.insert(new groundTurret(point(x * pixelSize, y * pixelSize)));
            }
            if(pix[x][y] == FLYINGTURRET)
            {
                enemies.insert(new turret(point(x * pixelSize, y * pixelSize)));
            }
            if(pix[x][y] == DRONE)
            {
                enemies.insert(new drone(point(x * pixelSize, y * pixelSize)));
            }
        }
    }
    for(int x = 0; x < w; x ++)
    {
        delete pix[x];
    }
    delete pix;
}

void game::focus()
{
    sceneId = GAME;
    Mix_FadeInMusic(music, -1, 1000);
    Mix_VolumeMusic(MIX_MAX_VOLUME);
}

void game::handleEvent(SDL_Event e)
{

    if(e.type == SDL_KEYUP)
    {
        if(e.key.keysym.sym == SDLK_ESCAPE)
        {
            Mix_FadeOutMusic(500);
            scenes[MENU]->focus();
        }
    }
}
void game::handleKeyState(const Uint8 *k)
{
    if(pl.since_death != -1)    return;

    if(k[SDL_SCANCODE_DOWN])
    {
        if(!pl.crouching)
        {
            pl.crouching = true;
        }
    } else {
        if(pl.crouching)
        {
            pl.crouching = false;
        }
    }

    pl.ignorePlatform = false;
    if(k[SDL_SCANCODE_Z])
    {
        if(pl.crouching)
        {
            pl.ignorePlatform = true;
        } else {
            if(pl.on_ground && pl.jump_ready == 0)
            {
                pl.jumping_left = 25;
            }
        }
    } else {
        pl.jumping_left = 0;
    }

    if(!(k[SDL_SCANCODE_LEFT] ^ k[SDL_SCANCODE_RIGHT]))
    {
        pl.vel.x = 0;
    } else {
        if(k[SDL_SCANCODE_RIGHT])
        {
            pl.vel.x = 1.5 * pixelSize;
            pl.face.x = 1;
        }
        if(k[SDL_SCANCODE_LEFT])
        {
            pl.vel.x = -1.5 * pixelSize;
            pl.face.x = -1;
        }
    }

    if(k[SDL_SCANCODE_UP])
    {
        pl.face.y = -1;
    } else {
        pl.face.y = 0;
    }

    if(k[SDL_SCANCODE_X] && pl.shoot_cd == 0)
    {
        projectile *bul = new bullet();
        if(pl.face.y == 0) bul->pos = pl.pos + point(pl.face.x * pl.size.x / 2, pl.face.y * pl.size.y / 2);
        else bul->pos = pl.pos + point(0.7 * pl.face.x * pl.size.x / 2, pl.face.y * pl.size.y / 2);
        if(pl.face.y == 0) bul->dir = pl.face.normalize();
        else    bul->dir = point(0, -1);
        bul->speed = 5;
        bullets.insert(bul);
        pl.shoot_cd = pl.max_shoot_cd;
    }

    if(k[SDL_SCANCODE_C] && pl.shoot_cd == 0)
    {
        projectile *sw = new swing();
        sw->dir = point(pl.face.x, 0);
        sw->pos = pl.pos + point(pl.size.x / 2 * pl.face.x, 0);
        bullets.insert(sw);
        pl.swing = 15;
        pl.shoot_cd = pl.max_shoot_cd;
    }
}

void game::draw()
{

    SDL_Rect src, target;
    SDL_Texture *tex;

    SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(gRenderer);

    target = {-offset.x, -offset.y, 2000 * pixelSize, 1000 * pixelSize};
    SDL_RenderCopy(gRenderer, background, NULL, &target);

    SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0x00, 0xff);

    for(auto i: platforms)
    {
        if(!i.vertical)
        {
            target = {i.upper_left.x - offset.x, i.upper_left.y - offset.y, i.length, 9 * pixelSize};
        } else {
            target = {i.upper_left.x - offset.x, i.upper_left.y - offset.y, 9 * pixelSize, i.length};
        }
        i.getSprite(tex, src);
        SDL_RenderCopy(gRenderer, tex, &src, &target);
    }

    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xff);
    for(auto i: walls)
    {
        if(!i.vertical)
        {
            target = {i.upper_left.x - offset.x, i.upper_left.y - offset.y, i.length, pixelSize};
        } else {
            target = {i.upper_left.x - offset.x, i.upper_left.y - offset.y, pixelSize, i.length};
        }
        //SDL_RenderFillRect(gRenderer, &target);
    }

    for(auto i: enemies)
    {
        target = {i->pos.x - i->size.x / 2 - offset.x, i->pos.y - i->size.y / 2 - offset.y, i->size.x, i->size.y};
        i->getSprite(tex, src);
        SDL_RenderCopy(gRenderer, tex, &src, &target);
    }

    for(auto i: dead)
    {
        target = {i->pos.x - i->size.x / 2 - offset.x, i->pos.y - i->size.y / 2 - offset.y, i->size.x, i->size.y};
        i->getDeathSprite(tex, src);
        SDL_RenderCopy(gRenderer, tex, &src, &target);
    }

    if(pl.since_death == -1)
    {
        target = {pl.pos.x - pl.size.x / 2 - offset.x, pl.pos.y - pl.size.y / 2 - offset.y, pl.size.x, pl.size.y};
        pl.getSprite(tex, src);
        SDL_RenderCopy(gRenderer, tex, &src, &target);
    }


    for(auto i: bullets)
    {
        target = {i->pos.x - i->size.x / 2 - offset.x, i->pos.y - i->size.y / 2 - offset.y, i->size.x, i->size.y};
        i->getSprite(tex, src);
        SDL_RenderCopy(gRenderer, tex, &src, &target);
    }
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xff, 0xff, 0xff);
    for(auto i: enemyBullets)
    {
        target = {i->pos.x - i->size.x / 2 - offset.x, i->pos.y - i->size.y / 2 - offset.y, i->size.x, i->size.y};
        i->getSprite(tex, src);
        SDL_RenderCopy(gRenderer, tex, &src, &target);
    }


    ///UI

    if(pl.since_death == -1)
    {
        int hp_size = 2;
        point php = point((gWidth - pl.maxhp * hp_size * pixelSize) / 2, gHeight - 20 * pixelSize);

        target = {php.x, php.y, pl.maxhp * hp_size * pixelSize, 16 * pixelSize};
        SDL_SetRenderDrawColor(gRenderer, 0x15, 0x6e, 0x71, 0xff);
        SDL_RenderFillRect(gRenderer, &target);
        target = {php.x, php.y, pl.hp * hp_size * pixelSize, 16 * pixelSize};
        SDL_SetRenderDrawColor(gRenderer, 0x4c, 0xf9, 0xff, 0xff);
        SDL_RenderFillRect(gRenderer, &target);

        tex = healthbar;
        target = {php.x - 7 * pixelSize, php.y - 2 * pixelSize, 12 * pixelSize, 20 * pixelSize};
        src = {0,0,12,20};
        SDL_RenderCopy(gRenderer, tex, &src, &target);
        target = {php.x + 5 * pixelSize, php.y - 2 * pixelSize, (pl.maxhp * hp_size - 10) * pixelSize, 20 * pixelSize};
        src = {12,0,1,20};
        SDL_RenderCopy(gRenderer, tex, &src, &target);
        target = {php.x + (pl.maxhp * hp_size - 5) * pixelSize, php.y - 2 * pixelSize, 12 * pixelSize, 20 * pixelSize};
        src = {21,0,12,20};
        SDL_RenderCopy(gRenderer, tex, &src, &target);

    }

    if(pl.since_death > 300)
    {
        target = {(gWidth - 110 * pixelSize) / 2, gHeight - 30 * pixelSize, 110 * pixelSize, 20 * pixelSize};
        SDL_RenderCopy(gRenderer, death_screen, NULL, &target);
    }

    SDL_RenderPresent(gRenderer);
}

void game::update()
{

    if(pl.since_death == -1) pl.update(pl, bullets);

    if(pl.jumping_left > 0)
    {
        pl.vel.y = -2 * pixelSize;
        pl.jumping_left --;
    }

    pl.pos = pl.vel + pl.pos;

    if(pl.pos.y + pl.hitbox.y / 2 > 1000 * pixelSize)
    {
        pl.takeDmg(20);
        pl.pos = pl.last_ground;
    }

    if(pl.pos.x - offset.x > 0.65 * gWidth)
    {
        offset.x = pl.pos.x - 0.65 * gWidth;
    }
    if(pl.pos.x - offset.x < 0.35 * gWidth)
    {
        offset.x = pl.pos.x - 0.35 * gWidth;
    }
    if(pl.pos.y - offset.y > 0.65 * gHeight)
    {
        offset.y = pl.pos.y - 0.65 * gHeight;
    }
    if(pl.pos.y - offset.y < 0.55 * gHeight)
    {
        offset.y = pl.pos.y - 0.55 * gHeight;
    }


    if(offset.y > 1000 * pixelSize - gHeight)
    {
        offset.y = 1000 * pixelSize - gHeight;
    }
    if(offset.x > 2000 * pixelSize - gWidth)
    {
        offset.x = 2000 * pixelSize - gWidth;
    }
    if(offset.x < 0)
    {
        offset.x = 0;
    }
    if(offset.y < 0)
    {
        offset.y = 0;
    }



    if(pl.jumping_left == 0)
    {
        pl.vel.y += 0.1 * pixelSize;
    }
    if(pl.vel.y > 2 * pixelSize)    pl.vel.y = 2 * pixelSize;


    bool on_ground = false;

    if(pl.vel.y > 0 && !pl.ignorePlatform)
    {
        for(auto i: platforms)
        {
            if(pl.pos.x >= i.upper_left.x - pl.hitbox.x / 2 && pl.pos.x <= i.upper_left.x + i.length + pl.hitbox.x / 2 &&
               i.upper_left.y <= pl.pos.y + pl.hitbox.y / 2 && pl.pos.y + pl.hitbox.y / 2 <= i.upper_left.y + 5)
            {
                pl.vel.y = 0;
                pl.pos.y = i.upper_left.y - pl.hitbox.y / 2;
                on_ground = true;
            }
        }
    }

    for(auto i: walls)
    {
        if(!i.vertical && pl.pos.x >= i.upper_left.x - pl.hitbox.x / 2 && pl.pos.x <= i.upper_left.x + i.length + pl.hitbox.x / 2 &&
            i.upper_left.y <= pl.pos.y + pl.hitbox.y / 2 && pl.pos.y + pl.hitbox.y / 2 <= i.upper_left.y + i.thickness)
        {
            pl.vel.y = 0;
            pl.pos.y = i.upper_left.y - pl.hitbox.y / 2;
            on_ground = true;
        }
        if(i.vertical && !i.reversed &&
            pl.pos.y > i.upper_left.y - pl.hitbox.y / 2 && pl.pos.y < i.upper_left.y + i.length + pl.hitbox.y / 2 &&
            i.upper_left.x <= pl.pos.x + pl.hitbox.x / 2 && pl.pos.x + pl.hitbox.x / 2 <= i.upper_left.x + i.thickness)
        {
            pl.vel.x = 0;
            pl.pos.x = i.upper_left.x - pl.hitbox.x / 2;
        }
        if(i.vertical && i.reversed &&
            pl.pos.y > i.upper_left.y - pl.hitbox.y / 2 && pl.pos.y < i.upper_left.y + i.length + pl.hitbox.y / 2 &&
            i.upper_left.x <= pl.pos.x - pl.hitbox.x / 2 && pl.pos.x - pl.hitbox.x / 2 <= i.upper_left.x + i.thickness)
        {
            pl.vel.x = 0;
            pl.pos.x = i.upper_left.x + pl.hitbox.x / 2 + i.thickness;
        }
    }

    if(on_ground)
    {
        pl.on_ground = true;
        pl.last_ground = pl.pos;
        if(pl.jump_ready) pl.jump_ready --;
    }
    if(!on_ground)
    {
        pl.on_ground = false;
        pl.jump_ready = 15;
    }

    for(auto i = bullets.begin(); i != bullets.end();)
    {
        (*i)->update(pl);
        for(auto j = enemies.begin(); j != enemies.end(); j++)
        {
            if((*i)->colision((*j)->pos, (*j)->size))
            {
                (*j)->takeDmg((*i)->dmg);
                (*i)->destroy = true;
                break;
            }
        }
        if((*i)->destroy)
        {
            delete *i;
            i = bullets.erase(i);
        } else {
            i ++;
        }
    }

    for(auto i = enemyBullets.begin(); i != enemyBullets.end();)
    {
        (*i)->update(pl);

        if((*i)->colision(pl.pos, pl.hitbox))
        {
            pl.takeDmg((*i)->dmg);
            if(pl.hp > pl.maxhp)    pl.hp = pl.maxhp;
            (*i)->destroy = true;
        }

        if((*i)->destroy)
        {
            delete *i;
            enemyBullets.erase(i++);
        } else {
            i ++;
        }
    }

    for(auto i = enemies.begin(); i != enemies.end();)
    {
        if(SDL_abs(pl.pos.x - (*i)->pos.x) > gWidth * 0.75 || SDL_abs(pl.pos.y - (*i)->pos.y) > gHeight * 0.75)
        {
            i++;
            continue;
        }
        (*i)->update(pl, enemyBullets);
        if((*i)->since_death != -1)
        {
            dead.insert(*i);
            i = enemies.erase(i);
        } else {
            i ++;
        }
    }

    for(auto i = dead.begin(); i != dead.end();)
    {
        (*i)->update(pl, enemyBullets);
        if((*i)->destroy)
        {
            i = dead.erase(i);
        } else {
            i ++;
        }
    }

    if(pl.hp <= 0)
    {
        if(pl.since_death == -1)
        {
            static_cast<menu*>(scenes[MENU])->new_game = true;
            pl.since_death = 0;
            pl.ignorePlatform = false;
            pl.vel.x = 0;
            dead.insert(&pl);
        }
        pl.hp = 0;
    }
}
