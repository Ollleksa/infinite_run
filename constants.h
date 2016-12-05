#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

using namespace std;

//The atributes of the screen
const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;
const int SCREEN_BPP = 32;

//The atributes of the background
const int LEVEL_WIDTH = 2040;
const int LEVEL_HEIGHT = 768;

//The atributes of block
const int BLOCK_SIZE = 160;

//Number of blocks
const int NUM_BLOCKS = 5;

//The artibutes of drone
const int DRONE_SIZE = 128;

//Gravity in game (Y axe go up)
const int GRAVITY = -5;

bool check_collision(SDL_Rect &A, SDL_Rect &B);

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL);

#endif
