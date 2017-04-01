#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <inttypes.h>
#include "map.h"

void colorsInit( )
{

	uint16_t fg, bg;

	for ( fg = 0; fg < 8; fg++ )
		for ( bg = 0; bg < 8; bg++ )
			init_pair( fg * 8 + bg, fg, bg );
}

uint16_t gameLoop( )
{

	//Main game loop
	uint8_t run = 1;
	uint16_t px, py;

	//Full draw
	//clear( );
	//mapDraw( );

	while( run )
	{
		clear( );
		mapDraw( );

		getPlayerPos( &px, &py ); //Player position

		switch( tolower( getch( ) ) ) //Move player tile depending on key pressed
		{
			case 'a':
				tileMove( px, py, -1, 0 );
				break;

			case 'd':
				tileMove ( px, py, 1, 0 );
				break;

			case 'w':
				tileMove ( px, py, 0, -1 );
				break;

			case 's':
				tileMove ( px, py, 0, 1 );
				break;

			case 'q':
				run = 0;
				break;
		}
	}
	return 0;
}

int main( int argc, char **argv )
{
	if( initscr() == NULL )
	{
		fprintf(stderr, "error: cannot run ncurses mode\n");
		return 0;
	}
	if( has_colors( ) == FALSE )
	{
		fprintf( stderr, "error: terminal doesn't support colors\n" );
	}
	start_color();
	curs_set( 0 );
	noecho( );
	colorsInit( );
	mapInit( );
	mapDraw( );
	mapLoad( argv[1] );
	gameLoop( );
	endwin( );
	return 0;
}
