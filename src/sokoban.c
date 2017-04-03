#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <inttypes.h>
#include "map.h"

void gfxEnd( )
{
	curs_set( 1 );
	echo( );
	endwin( );
}

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

		if ( checkWinner( ) == 1 )
		{
			gfxEnd( );
			printf("You won $$$\n");
			return 10;
		}

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
	uint16_t ec = 0;
	if( initscr() == NULL )
	{
		return 1;
	}
	if( has_colors( ) == FALSE )
	{
		return 1;
	}
	start_color();
	curs_set( 0 );
	noecho( );
	colorsInit( );
	mapInit( );
	mapDraw( );
	if ( ( ec = mapLoad( argv[1] ) ) != MAP_OK)
	{
		gfxEnd( );
		switch ( ec )
		{
			case MAP_ERR_PCNT:
				fprintf(stderr, "err %d: wrong player count!\n", ec);
				break;

			case MAP_ERR_FILE:
				fprintf(stderr, "err %d: cannot open level file!\n", ec );
				break;

			case MAP_ERR_SIZE:
				fprintf(stderr, "err %d: wrong map size!\n", ec );
		}
		return 1;
	}
	gameLoop( );
	gfxEnd( );
	return 0;
}
