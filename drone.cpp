#include "drone.h"

//usable variables defined in main.cpp
extern SDL_Surface* screen;
extern SDL_Surface* background;
extern SDL_Surface* drone;
extern SDL_Surface* info;
extern SDL_Surface* eva;

extern SDL_Rect camera;

//Events queue
extern SDL_Event event;

//The font
extern TTF_Font *font;
extern SDL_Color textColor;

//losing status
extern bool loser;

Drone::Drone()
{
    box.x = SCREEN_WIDTH/2 + DRONE_SIZE;
    box.y = SCREEN_HEIGHT/2;
    box.w = DRONE_SIZE;
    box.h = DRONE_SIZE;

    x = box.x;
    y = box.y;
    Vx = 0;
    Vy = 0;
    ax = 0;
    ay = 0;
}

void Drone::handle_keys()
{
    //If key was pressed
    if( event.type == SDL_KEYDOWN)
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: ay--; break;
            case SDLK_DOWN: ay++; break;
            case SDLK_LEFT: ax--; break;
            case SDLK_RIGHT: ax++; break;
            default: ;
        }
    }
    //If key was relized
    else if( event.type == SDL_KEYUP )
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: ay++; break;
            case SDLK_DOWN: ay--; break;
            case SDLK_LEFT: ax++; break;
            case SDLK_RIGHT: ax--; break;
            default: ;
        }
    }
}

void Drone::move(int dt, Block *firstblocks[], Block *nextblocks[])
{
    Vx += 10 * ax * (dt/1000.f);
    Vy += (10 * ay-GRAVITY) * (dt/1000.f);

    float dx, dy;
    dx = Vx* (dt/1000.f);
    dy = Vy* (dt/1000.f);

    x += dx;
    y += dy;

    box.x = x;
    box.y = y;

    if( (box.x < 0) /*|| ( box.x > LEVEL_WIDTH )*/ )
    {
        x -= dx;
        Vx = 0;
    }

    if( (box.y < 0) || ( box.y > LEVEL_HEIGHT - DRONE_SIZE ) )
    {
        y -= dy;
        Vy = 0;
        loser = true;
    }

    for(int i = 0; i < NUM_BLOCKS; i++)
    {
	SDL_Rect one = firstblocks[i]->get_box();
    	if( check_collision(box, one) )
    	{
		impulse(one, dx, dy);
        }

	SDL_Rect two = nextblocks[i]->get_box();
    	if( check_collision(box, two) )
    	{
		impulse(two, dx, dy);
        }
    }

    box.x = x;
    box.y = y;
}

void Drone::impulse(SDL_Rect obstacle, float dx, float dy)
{
	bool A, B;
    	SDL_Rect temp;

    	temp.h = box.h;
    	temp.w = box.w;

        temp.x = x;
        temp.y = y - dy;
        A = check_collision(temp, obstacle);

        temp.x = x - dx;
        temp.y = y;
        B = check_collision(temp, obstacle);

        if(A == false && B == true)
        {
            x -= dx;
            Vx = -Vx;
        }
        else if(A == true && B == false)
        {
            y -= dy;
            Vy = -Vy;
        }
        else if(A == false && B == false)
        {
            if(Vx < Vy)
            {
                x -= dx;
                Vx = -Vx;
            }
            else
            {
                y -= dy;
                Vy = -Vy;
            }
        }
        else
        {
            x -= dx;
            Vx = -Vx;
            y -= dy;
            Vy = -Vy;
        }
}

void Drone::show()
{
    //apply_surface(box.x - camera.x, box.y - camera.y, drone, screen );
    apply_surface(box.x - camera.x, box.y - camera.y, drone, screen );

    //create strings
    stringstream coord1, coord2;
    coord1 << "X: " << box.x;
    coord2 << "Y: " << box.y;
    //create text
    info = TTF_RenderText_Solid( font, coord1.str().c_str(), textColor );
    apply_surface(0,0, info, screen );
    info = TTF_RenderText_Solid( font, coord2.str().c_str(), textColor );
    apply_surface(0,32, info, screen );
}

void Drone::set_camera()
{
    camera.x = box.x - SCREEN_WIDTH/2;
    camera.y = box.y - SCREEN_HEIGHT/2;

    //keep camera in bounds
    if(camera.x < 0)
    {
        camera.x = 0;
    }
    //if(camera.x > LEVEL_WIDTH - camera.w)
    //{
    //    camera.x = LEVEL_WIDTH - camera.w;
    //}

    if(camera.y < 0)
    {
        camera.y = 0;
    }
    if(camera.y > LEVEL_HEIGHT - camera.h)
    {
        camera.y = LEVEL_HEIGHT - camera.h;
    }
}
