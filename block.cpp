#include "block.h"

extern SDL_Rect camera;
extern SDL_Surface* eva;
extern SDL_Surface* screen;

Block::Block(int x, int y)
{
    wood.x = x;
    wood.y = y;
    wood.h = BLOCK_SIZE;
    wood.w = BLOCK_SIZE;
}

SDL_Rect Block::get_box()
{
    return wood;
}

void Block::show_block()
{
    if( check_collision(camera, wood) == true )
    {
    	apply_surface(wood.x-camera.x,wood.y-camera.y,eva,screen);
    }
}
