#ifndef BUTTON_H
#define BUTTON_H

#include "constants.h"
#include "SDL/SDL.h"

class Button 
{
	private: 
	//The attributes of the button 
	SDL_Rect box;
	//Type of button
	int b_type;
	//The part of the button sprite sheet that will be shown 
	SDL_Rect* clip; 
	public: 
	//Initialize the variables 
	Button( int x, int y, int w, int h, int btype); 
	//Handles events and set the button's sprite region 
	void handle_events(); 
	//Shows the button on the screen 
	void show_butt(); 
};

void set_clips();

#endif
