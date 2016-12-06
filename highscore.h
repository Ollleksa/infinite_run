#ifndef HIGHTSCORE_H
#define HIGHTSCORE_H

#include "constants.h"
#include <string>
#include <fstream>
#include <sstream>

//The key press interpreter 
class StringInput 
{ 
	private: 
	//The storage string 
	string str; 
	//The text surface 
	SDL_Surface *text; 
	public: 
	//Initializes variables 
	StringInput(); 
	//Does clean up 
	~StringInput(); 
	//Handles input 
	void handle_input(); 
	//Shows the message on screen 
	void show_centered(); 
	//get string
	string get_name();
};

class SaveLoad
{
	private:
	//name from file
	string str;
	int score;
	//output message
	SDL_Surface *name;
	public:
	SaveLoad();
	~SaveLoad();
	//get information from file 
	int load_high();
	//save into file
	void save_high(int score, string str);
	//show messages
	void show_high_beaten();
	void show_high_unbeaten();
};

#endif
