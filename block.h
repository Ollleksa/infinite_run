#ifndef BLOCK_H
#define BLOCK_H

#include "SDL/SDL.h"
#include "constants.h"

class Block
{
    private:
        SDL_Rect wood;
    public:
        Block(int x, int y);
        void show_block();
        SDL_Rect get_box();
};

#endif
