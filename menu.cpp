#include"connector.h"



menu::menu()
{
    main_menu = loadTexture("img\\menu.png");
    controls = loadTexture("img\\controls.png");
    selector = loadTexture("img\\menu_selector.png");


    music = Mix_LoadWAV("music\\8bit.wav");

    init();
}

enum menuOptions{
    START,
    CONTROLS,
    NUM_OPTIONS
};

void menu::init()
{

    ctrl = false;

    option_coord.push_back(point(15, 32));
    option_coord.push_back(point(15, 52));
    option_size = point(98, 16);
    curr_option = START;

    new_game = true;

    //channel = Mix_PlayChannel(-1, music, -1);
    //Mix_Pause(channel);
    //std::cout << Mix_Playing(channel);
}

void menu::focus()
{
    channel = Mix_FadeInChannel(-1, music, -1, 1000);
    sceneId = MENU;
}

void menu::handleEvent(SDL_Event e)
{
    if(e.type == SDL_KEYUP)
    {
        if(!ctrl)
        {
            if(e.key.keysym.sym == SDLK_RETURN)
            {
                if(curr_option == START)
                {
                    Mix_FadeOutChannel(channel, 500);
                    if(new_game)
                    {
                        scenes[GAME]->init();
                        new_game = false;
                    }
                    scenes[GAME]->focus();
                }
                if(curr_option == CONTROLS)
                {
                    ctrl = true;
                }
            }
        }
    }
    if(e.type == SDL_KEYDOWN)
    {
        if(!ctrl)
        {
            if(e.key.keysym.sym == SDLK_UP)
            {
                curr_option --;
                if(curr_option < 0) curr_option += NUM_OPTIONS;
            }
            if(e.key.keysym.sym == SDLK_DOWN)
            {
                curr_option ++;
                if(curr_option >= NUM_OPTIONS) curr_option -= NUM_OPTIONS;
            }
        } else {
            if(e.key.keysym.sym == SDLK_ESCAPE)
            {
                ctrl = false;
            }
        }
    }
}
void menu::handleKeyState(const Uint8 *k)
{

}

void menu::draw()
{

    SDL_Rect src, target;
    SDL_Texture *tex;

    if(!ctrl)
    {
        int frame = ((time / 10) / 5) % 2;
        if(frame == 0)  src = {0, 0, 128, 80};
        else    src = {0, 80, 128, 80};
        SDL_RenderCopy(gRenderer, main_menu, &src, NULL);

        target = {(option_coord[curr_option].x - 8) * 5, (option_coord[curr_option].y - 2) * 5, (option_size.x + 16) * 5, (option_size.y + 4) * 5};
        SDL_RenderCopy(gRenderer, selector, NULL, &target);
    } else {
        SDL_RenderCopy(gRenderer, controls, NULL, NULL);
    }


    SDL_RenderPresent(gRenderer);
}

void menu::update()
{

}
