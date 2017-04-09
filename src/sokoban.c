#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <inttypes.h>
#include <time.h>
#include <sys/time.h>
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
			wbkgd( stdscr, COLOR_PAIR( 3 ) );
			attron( COLOR_PAIR( 5 ) );
			mvprintw(map.height/2 - 3, map.width/2-9, "####################");
			mvprintw(map.height/2 - 2, map.width/2-9, "#     YOU  WON     #");
			mvprintw(map.height/2 - 1, map.width/2-9, "#        in        #");
			mvprintw(map.height/2    , map.width/2-9, "#    %04d  steps   #", stpcnt);
			mvprintw(map.height/2 + 2, map.width/2-9, "####################");
			mvprintw(map.height/2 + 1, map.width/2-9,"# Time : %ld.%02lds #\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
			attroff( COLOR_PAIR( 5 ) );
			attron( COLOR_PAIR( 3 ) );
			mvprintw(map.height/2 + 5, map.width/2-12," Press any key to exit...");
			attroff( COLOR_PAIR( 3 ) );
			getch( );
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
