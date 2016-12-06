#ifndef DRONE_H
#define DRONE_H

#include "constants.h"
#include "block.h"
#include <iostream>
#include <sstream>
#include <string>

class Drone
{
    private:
        SDL_Rect box;     //Box for our drone
        float x, y;
        float Vx, Vy;     //velocities
        float ax, ay;     //forses;

    public:
        Drone();
        //read keyboard
        void handle_keys();
        //Move system
        void move(int dt, Block *firstblocks[], Block *nextblocks[]);
	//Check impulses
	void impulse(SDL_Rect obstacle, float dx, float dy);
        //show system
        void show();
        //set camera in right position
        void set_camera();
	//get score (x-coordinate)
	int get_score();
};

#endif
