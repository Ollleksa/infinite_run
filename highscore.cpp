#include "highscore.h"

extern SDL_Surface* screen;

//Events queue
extern SDL_Event event;

//The font
extern TTF_Font *font;
extern SDL_Color textColor;

StringInput::StringInput()
{
    //all empty
    str = "";
    text = NULL;
    //enable unicode
    SDL_EnableUNICODE(SDL_ENABLE);
}

StringInput::~StringInput()
{
    SDL_FreeSurface( text );
    SDL_EnableUNICODE( SDL_DISABLE );
}

void StringInput::handle_input()
{
    if( event.type == SDL_KEYDOWN )
    {
        //keep current string
        string temp = str;
        if( str.length() <= 16 )
        {
            //If space
            if( event.key.keysym.unicode == (Uint16)' ')
            {
                str += (char)event.key.keysym.unicode;
            }
            //If number
            else if( (event.key.keysym.unicode >= (Uint16)'0') && (event.key.keysym.unicode <= (Uint16)'9') )
            {
                str += (char)event.key.keysym.unicode;
            }
            else if( (event.key.keysym.unicode >= (Uint16)'A') && (event.key.keysym.unicode <= (Uint16)'Z') )
            {
                str += (char)event.key.keysym.unicode;
            }
            else if( (event.key.keysym.unicode >= (Uint16)'a') && (event.key.keysym.unicode <= (Uint16)'z') )
            {
                str += (char)event.key.keysym.unicode;
            }
        }

        //If backspace, then cut string
        if( ( event.key.keysym.sym == SDLK_BACKSPACE ) && ( str.length() != 0 ) )
        {
            str.erase( str.length() - 1);
        }

        if( str != temp)
        {
            SDL_FreeSurface( text );
            //Render new
            text = TTF_RenderText_Solid( font, str.c_str(), textColor);
        }
    }
}

void StringInput::show_centered()
{
    if( text != NULL )
    {
        apply_surface( (SCREEN_WIDTH - text->w)/2, SCREEN_HEIGHT/2, text, screen);
    }
}

string StringInput::get_name()
{
	return str;
}

SaveLoad::SaveLoad()
{
	str = "";
    	name = NULL;
	score = 0;
}

SaveLoad::~SaveLoad()
{
	SDL_FreeSurface( name );
}

int SaveLoad::load_high()
{
	ifstream load( "game.s" );
	if ( load != NULL )
	{		
		load >> score;
		load.ignore();
		getline( load, str );

		return score;
	}
}

void SaveLoad::save_high(int score, string str)
{
	ofstream save( "game.s" );
	save << score;
	save << "\n";
	save << str;
}

void SaveLoad::show_high_unbeaten()
{
	stringstream text;
	text << str << "get " << score << " points";
	name = TTF_RenderText_Solid( font, text.str().c_str(), textColor );
	apply_surface((SCREEN_WIDTH - name->w)/2, (SCREEN_HEIGHT-name->h)/2 - 200, name, screen );
}

void SaveLoad::show_high_beaten()
{
	show_high_unbeaten();
	name = TTF_RenderText_Solid( font, "Good Work!", textColor );
	apply_surface((SCREEN_WIDTH - name->w)/2, (SCREEN_HEIGHT-name->h)/2 - 150, name, screen );
}
