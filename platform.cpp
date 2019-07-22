#include"platform.h"
#include"connector.h"

SDL_Texture* platform::texture = NULL;

platform::platform(point _ul, double _length, bool _vertical, bool _reversed)
{
    upper_left = _ul;
    length = _length;
    vertical = _vertical;
    reversed = _reversed;
    thickness = 5;
    if(!texture)
    {
        texture = loadTexture("img\\platform.png");
    }
}

void platform::getSprite( SDL_Texture *&_texture, SDL_Rect &_rect)
{
    _texture = texture;
    switch((time / 10) % 3)
    {
    case 0:
        _rect = {0,0,100,9};
        break;
    case 1:
        _rect = {0,9,100,9};
        break;
    case 2:
        _rect = {0,18,100,9};
        break;
    }
}
