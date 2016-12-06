#include "button.h"

extern SDL_Surface* butt;
extern SDL_Surface* screen;

SDL_Rect clips[ 4 ];

//Events queue
extern SDL_Event event;

//losing status
extern bool stage_q;
extern bool quit;

void set_clips()
{
    //clip sprites
    clips[MENU_BEGIN].x = 0;
    clips[MENU_BEGIN].y = 0;
    clips[MENU_BEGIN].w = 500;
    clips[MENU_BEGIN].h = 125;

    clips[MENU_EXIT].x = 0;
    clips[MENU_EXIT].y = 125;
    clips[MENU_EXIT].w = 500;
    clips[MENU_EXIT].h = 125;

    clips[LOSE_BEGIN].x = 0;
    clips[LOSE_BEGIN].y = 250;
    clips[LOSE_BEGIN].w = 500;
    clips[LOSE_BEGIN].h = 125;

    clips[LOSE_EXIT].x = 0;
    clips[LOSE_EXIT].y = 375;
    clips[LOSE_EXIT].w = 500;
    clips[LOSE_EXIT].h = 125;
}

Button::Button(int x, int y, int w, int h, int btype)
{
    box.x = x;
    box.y = y;
    box.w = w;
    box.h = h;

    b_type = btype;

    //sprite
    clip = &clips[b_type];
}

void Button::handle_events()
{
    //the mouse ofsets
    int x = 0, y = 0;

    if( (event.type == SDL_MOUSEBUTTONDOWN) && (event.button.button == SDL_BUTTON_LEFT))
    {
        //get mouse offsets
        x = event.button.x;
        y = event.button.y;
        //if mouse is over button
        if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
        {
            switch(b_type)
            {
                case 0: stage_q = true; break;
                case 1: stage_q = true; quit = true; break;
		case 2: stage_q = true;	break;
		case 3: stage_q = true; quit = true; break;
                default: ;
            }
        }
    }
}

void Button::show_butt()
{
    apply_surface( box.x, box.y, butt, screen, clip);
}


