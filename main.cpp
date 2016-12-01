#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include "drone.h"
#include "timer.h"
#include "block.h"
#include "constants.h"

SDL_Surface* screen = NULL;
SDL_Surface* background = NULL;
SDL_Surface* drone = NULL;
SDL_Surface* info = NULL;
SDL_Surface* eva = NULL;

//Camera and block
SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

//Events queue
SDL_Event event;

//The font
TTF_Font *font = NULL;
SDL_Color textColor = { 0, 0, 0};

//Load images from file. Improve them in some way (I do not know), and delete colorkey
SDL_Surface *load_image (string filename)
{
    //Temporary image and optimized one;
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
        SDL_FreeSurface( loadedImage );
    }

    //if( optimizedImage != NULL )
    //{
    //Map the color key //set all picsels of color R 0, G 0xFF, B 0xFF to be transparent
    //    Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF);
    //    SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorkey);
    //}

    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    //Create rectangle to hold the offsets
    SDL_Rect offset;

    //Give the offsets to rectangle
    offset.x = x;
    offset.y = y;

    //Blit the surface (some graphics shit), clip is for cutting images
    SDL_BlitSurface ( source, clip, destination, &offset );
}

bool init()
{
    //start SDL, if doesnt start - close all
	if( SDL_Init( SDL_INIT_EVERYTHING) == -1)
	{
        cout << "SDL_Init" << endl;
        return false;
	}

	//set up screen, if error - shut it down
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_FULLSCREEN);
    if (screen == NULL)
    {
        cout << "SDL_vIDEO" << endl;
        return false;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        cout << "TTF_Init" << endl;
        return false;
    }

    //set the windows caption
    SDL_WM_SetCaption("Game", NULL);

    //If everything is good
    return true;
}

bool load_files()
{
    //Background
    background = load_image("backgroung.png");
    //drone =/
    drone = load_image("drone.png");
    //block
    eva = load_image("block.png");
    //Font
    font = TTF_OpenFont("font.ttf", 36);

    //if problems, go down
    if( background == NULL )
        {cout << "background" << endl; return false;}

    if( font == NULL )
        {cout << "font" << endl; return false;}

    //If all alright
    return true;
}

void close(Block *blocks[])
{
    //Free the surfaces
    SDL_FreeSurface(background);
    SDL_FreeSurface( drone );
    SDL_FreeSurface( info );
    SDL_FreeSurface( eva );

    //Close font
    TTF_CloseFont( font );

    //Free the tiles
    for( int t = 0; t < 10; t++ )
    {
        delete blocks[t];
    }

    //Quit TTF and SDL
    TTF_Quit();
    SDL_Quit();
}

bool check_collision(SDL_Rect &A, SDL_Rect &B)
{
    //borders of rectangles
    int leftA, rightA, topA, botA;
    int leftB, rightB, topB, botB;

    //Numbers
    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    botA = A.y + A.h;

    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    botB = B.y + B.h;

    //check if A is out of B
    if( ( (botA <= topB) || (topA >= botB ) || (rightA <= leftB) || (leftA >= rightB) ) == false )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void show_scrolling()
{
    int pos;
    pos = (camera.x % background->w);

    apply_surface(-pos,0,background,screen);
    apply_surface(-pos+background->w,0,background,screen);
    //if you need to go left uncomment
    //apply_surface(-pos-background->w,0,background,screen);
}

bool block_creation( Block *blocks[] )
{
    srand(time(NULL));
    for(int i = 0; i < 10; i++)
    {
        int x, y;
        x = rand()%LEVEL_WIDTH;
        y = rand()%LEVEL_HEIGHT;
        blocks[i] = new Block(x, y);
    }

    return true;
}

int main(int argc, char* args[])
{
	//main loop flag
	bool quit = false;

	Drone myDrone;

	Block *blocks[10];

	Timer delta;

    //Initialize
    if( init() == false )
    {
        return 1;
    }
    if( load_files() == false )
    {
        return 1;
    }
    if( block_creation( blocks ) == false )
	{
        cout << "Bloks!!!" << endl;
        return 1;
    }

    //start timer first time
    delta.start();

    //while isnt closed
    while( !quit )
    {
        //If something happens
        while( SDL_PollEvent( &event ))
        {
            //Read the keyboard
            myDrone.handle_keys();

            //If escape was pressed
            if( ( event.type == SDL_KEYDOWN ) && ( event.key.keysym.sym == SDLK_ESCAPE ) )
            {
                quit = true;
            }
            //User request quit
            if( event.type == SDL_QUIT )
            {
                quit = true;
            }
        }

        //set camera
        myDrone.set_camera();

        //Moving
        myDrone.move(delta.get_Ticks(),blocks);

        //Restart timer for next iteration
        delta.start();

        //apply background part with camera
        show_scrolling();

        //Show block
        for(int i = 0; i < 10; i++)
        {
            blocks[i]->show_block();
        }

        myDrone.show();

        //Update screen
        if(SDL_Flip( screen ) == -1)
        {
            return 1;
        }
    }
	//quit SDL
	close(blocks);

	return 0;
}
