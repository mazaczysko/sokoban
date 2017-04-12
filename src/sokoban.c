#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <inttypes.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include "map.h"

int cntrPrnt( uint16_t y, const char* txt )
{
	return mvprintw( y, map.width/2 - strlen( txt )/2, txt );
}
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
	struct timeval tval_before, tval_after, tval_result;
	uint8_t run = 1;
	uint16_t px, py;
	uint16_t stpcnt = 0;

	//Full draw
	gettimeofday(&tval_before, NULL);

	while( run )
	{

		clear( );
		mapDraw( );

		if ( checkWinner( ) == 1 )
		{
			gettimeofday(&tval_after, NULL);
			timersub(&tval_after, &tval_before, &tval_result);
			clear( );
			wbkgd( stdscr, COLOR_PAIR( 24 ) );

			char *buf = (char*) malloc( map.width );
			if ( buf == NULL ) fprintf( stderr, "dupagównozjebałosię\n" );
			
			snprintf( buf, map.width, "YOU WON" ) && cntrPrnt( map.height/2-2 ,buf );
			snprintf( buf, map.width, "STEPS: %d", stpcnt ) && cntrPrnt( map.height/2 ,buf );
			snprintf( buf, map.width, "TIME: %ld.%02lds", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec ) && cntrPrnt( map.height/2+2 ,buf );
			getch( );
			attroff( COLOR_PAIR( 24 ) );
			return 0;
		}
		getPlayerPos( &px, &py ); //Player position

		switch( tolower( getch( ) ) ) //Move player tile depending on key pressed
		{
			case 'a':
				stpcnt += tileMove( px, py, -1, 0 );
				break;

			case 'd':
				stpcnt += tileMove ( px, py, 1, 0 );
				break;

			case 'w':
				stpcnt += tileMove ( px, py, 0, -1 );
				break;

			case 's':
				stpcnt += tileMove ( px, py, 0, 1 );
				break;

			case 'q':
				run = 0;
				break;

			default:
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
