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
#include "button.h"
#include "highscore.h"

SDL_Surface* screen = NULL;
SDL_Surface* background = NULL;
SDL_Surface* drone = NULL;
SDL_Surface* info = NULL;
SDL_Surface* eva = NULL;
SDL_Surface* menu = NULL;
SDL_Surface* butt = NULL;

//Camera and block
SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

//Events queue
SDL_Event event;

//The font
TTF_Font *font = NULL;
SDL_Color textColor = { 0, 0, 0};

int current_screen = 0;
int space_for_block = LEVEL_WIDTH / NUM_BLOCKS;

//losing status
bool loser = false;
//main loop flag
bool quit = false;
bool stage_q = false;

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

    if( optimizedImage != NULL )
    {
    //Map the color key //set all picsels of color R 0, G 0xFF, B 0xFF to be transparent
        Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF);
        SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorkey);
    }

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
    //menu
    menu = load_image("menu.png");
    //Background
    background = load_image("backgroung.png");
    //drone =/
    drone = load_image("drone.png");
    //block
    eva = load_image("block.png");

    butt = load_image( "buttons2.png" );
    //Font
    font = TTF_OpenFont("font.ttf", 36);

    //if problems, go down
    if( menu == NULL )
        {cout << "background" << endl; return false;}

    if( font == NULL )
        {cout << "font" << endl; return false;}

    //If all alright
    return true;
}

void free_surf()
{
    //Free the surfaces
    SDL_FreeSurface(background);
    SDL_FreeSurface( drone );
    SDL_FreeSurface( info );
    SDL_FreeSurface( eva );
    SDL_FreeSurface( menu );
    SDL_FreeSurface( butt );

    //Close font
    TTF_CloseFont( font );

    //Quit TTF and SDL
    TTF_Quit();
    SDL_Quit();

}

void close(Block *firstblocks[], Block *nextblocks[])
{
    //Free the tiles
    for( int t = 0; t < NUM_BLOCKS; t++ )
    {
        delete firstblocks[t];
        delete nextblocks[t];
    }

    free_surf();
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

bool block_creation( Block *blocks[], int level )
{
    for(int i = 0; i < NUM_BLOCKS; i++)
    {
        int x, y;
        x = rand()%(space_for_block-BLOCK_SIZE) + level*LEVEL_WIDTH + i*space_for_block;
        y = rand()%(LEVEL_HEIGHT-BLOCK_SIZE);
        blocks[i] = new Block(x, y);
    }

    return true;
}

void block_creation_first( Block *blocks[] )
{
    blocks[0] = new Block(-BLOCK_SIZE, -BLOCK_SIZE);
    for(int i = 1; i < NUM_BLOCKS; i++)
    {
        int x, y;
        x = rand()%(space_for_block-BLOCK_SIZE) + i*space_for_block;
        y = rand()%(LEVEL_HEIGHT-BLOCK_SIZE);
        blocks[i] = new Block(x, y);
    }
}

void block_destruction(Block *blocks[])
{
    for( int t = 0; t < NUM_BLOCKS; t++ )
    {
        delete blocks[t];
    }
}

void show_block( Block *firstblocks[], Block *nextblocks[] )
{
    int screen_number = camera.x/LEVEL_WIDTH;

    if(current_screen != screen_number)
    {
        for(int i = 0; i < NUM_BLOCKS; i++)
        {
            firstblocks[i] = nextblocks[i];
        }
        if( block_creation( nextblocks, screen_number+1 ) == false )
        {
            cout << "Bloks!!!" << endl;
        }
    }

    for(int i = 0; i < NUM_BLOCKS; i++)
    {
        firstblocks[i]->show_block();
        nextblocks[i]->show_block();
    }

    current_screen = screen_number;
}

void show_result( int score)
{
    stringstream text;
    text << "You get " << score << " points";
    info = TTF_RenderText_Solid( font, text.str().c_str(), textColor );
    apply_surface((SCREEN_WIDTH - info->w)/2, (SCREEN_HEIGHT-info->h)/2 - 100, info, screen );
    info = TTF_RenderText_Solid( font, "Type yor name: ", textColor );
    apply_surface((SCREEN_WIDTH - info->w)/2, (SCREEN_HEIGHT-info->h)/2 - 50, info, screen );
}

int main(int argc, char* args[])
{
	set_clips();

	Block *firstblocks[NUM_BLOCKS];
	Block *nextblocks[NUM_BLOCKS];

	Timer delta;

	Button MStart(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 500, 125, 0);
    Button MExit(SCREEN_WIDTH/2, SCREEN_HEIGHT/2+130, 500, 125, 1);
    Button LStart(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 500, 125, 2);
    Button LExit(SCREEN_WIDTH/2, SCREEN_HEIGHT/2+130, 500, 125, 3);

    //Initialize
    if( init() == false )
    {
        return 1;
    }
    if( load_files() == false )
    {
        return 1;
    }

    while(!stage_q)
    {
        apply_surface(0, 0, menu, screen);
        MStart.show_butt();
        MExit.show_butt();

        while( SDL_PollEvent( &event ))
        {
            MStart.handle_events();
            MExit.handle_events();
            //if space was pressed
            if(( event.type == SDL_KEYDOWN ) && ( event.key.keysym.sym == SDLK_SPACE ))
            {
                stage_q = true;
            }
            //If escape was pressed
            if( ( event.type == SDL_KEYDOWN ) && ( event.key.keysym.sym == SDLK_ESCAPE ) )
            {
                stage_q = true;
                quit = true;
            }
            //User request quit
            if( event.type == SDL_QUIT )
            {
                stage_q = true;
                quit = true;
            }
        }

        SDL_Flip( screen );
    }

    if(quit == true)
    {
        free_surf();
        return 1;
    }

    //while isnt closed
    do
    {
        loser = false;
        srand(time(NULL));
        block_creation_first(firstblocks);
        if( block_creation( nextblocks, 1 ) == false )
        {
            cout << "Bloks!!!" << endl;
        }
        Drone myDrone;
        StringInput SImput;
	SaveLoad SL;
        int score = 0;
	bool beaten = false;
        stage_q = false;
        //start timer first time
        delta.start();

        while( !stage_q )
        {
            //If something happens
            while( SDL_PollEvent( &event ))
            {
                //Read the keyboard
                myDrone.handle_keys();

                //If escape was pressed
                if( ( event.type == SDL_KEYDOWN ) && ( event.key.keysym.sym == SDLK_ESCAPE ) )
                {
                    stage_q = true;
                }
                //User request quit
                if( event.type == SDL_QUIT )
                {
                    stage_q = true;
                    quit = true;
                }
            }

            //set camera
            myDrone.set_camera();

            //Moving
            myDrone.move(delta.get_Ticks(),firstblocks,nextblocks);

            //Restart timer for next iteration
            delta.start();

            //apply background part with camera
            show_scrolling();

            //Show block
            show_block(firstblocks,nextblocks);

            myDrone.show();

            if(loser == true)
            {
                stage_q = true;
            }
            //Update screen
            if(SDL_Flip( screen ) == -1)
            {
                return 1;
            }
        }

        //end screen shown
        score = myDrone.get_score();
        stage_q = false;
	if( SL.load_high() < score)
	 {
		beaten = true;
	 }
        while ( (!stage_q) && (!quit) )
        {
            while( SDL_PollEvent( &event ))
            {
                SImput.handle_input();
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

                if( ( event.type == SDL_KEYDOWN ) && ( event.key.keysym.sym == SDLK_RETURN ) )
                {
                    stage_q = true;
                }
            }

       	 apply_surface(0,0,menu,screen);
         show_result(score);
         SImput.show_centered();
	if(beaten)
	{
		SL.show_high_beaten();
	}
	else
	{
		SL.show_high_unbeaten();
	}
         SDL_Flip(screen);
        }
	
	if( beaten )
	 {
		SL.save_high(score, SImput.get_name());
	 }
	
        stage_q = false;
        while ( !stage_q && !quit )
        {
            SDL_SetAlpha(menu, SDL_SRCALPHA, 128);
            LStart.show_butt();
            LExit.show_butt();
            SDL_Flip(screen);

            while( SDL_PollEvent( &event ))
            {
                LStart.handle_events();
                LExit.handle_events();
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
        }
    }
    while(!quit);

	//quit SDL
	close(firstblocks, nextblocks);

	return 0;
}
