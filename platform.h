#ifndef PLATFORM_H
#define PLATFORM_H
#include"point.h"
#include<SDL.h>


class platform
{
public:
    point upper_left;
    double length;
    bool vertical;
    bool reversed;
    static SDL_Texture *texture;
    double thickness;
    platform(point _ul, double _length, bool _vertical = 0, bool _reversed = 0);
    void getSprite( SDL_Texture*& texture, SDL_Rect &rect);
};

#endif // PLATFORM_H
