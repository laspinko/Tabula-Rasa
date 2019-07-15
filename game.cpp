#include"connector.h"

void game::init()
{

}

void game::handleEvent(SDL_Event e)
{
    if(e.type != SDL_MOUSEMOTION)    return;
    mouseX = e.motion.x;
    mouseY = e.motion.y;
}

void game::draw()
{

    SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
    //SDL_RenderClear(gRenderer);


    SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0x00, 0xff);
    SDL_RenderDrawPoint(gRenderer, mouseX, mouseY);

    SDL_RenderPresent(gRenderer);
}
