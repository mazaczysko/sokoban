#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <inttypes.h>

typedef struct
{
	uint16_t id;
	uint8_t fg, bg;
	char chr;

} Tile;
//								ID	FG	BG	CHAR
const Tile TILE_AIR = 			{0,	0,	0,	' ' };
const Tile TILE_PLAYER = 		{1,	7,	0,	'@' };
const Tile TILE_SOCKET = 		{2,	0,	4,	' '	};
const Tile TILE_BOX = 			{3,	2,	0,	'#' };
const Tile TILE_SOCKETPLAYER = 	{4,	7,	4,	'@' };
const Tile TILE_SOCKETBOX = 	{5,	3,	5,	'#'	};
const Tile TILE_WALL = 			{6,	0,	6,	' ' };

struct map
{
	uint16_t width, height;
	Tile **map;
} map;


void mapInit()
{
	uint16_t i;
	getmaxyx(stdscr, map.height, map.width );
	map.map = (Tile**) malloc( map.width * sizeof( Tile* ) );
	for( i = 0; i < map.width; i++)
		map.map[i] = (Tile*) malloc( map.height * sizeof( Tile ) );

}

void mapDraw()
{
	uint16_t i, j;
	for( i = 0; i < map.height; i++ )
	{
		for( j = 0; j < map.width; j++ )
			mvprintw( i, j, "%c", map.map[j][i].chr );
	}
}

int main()
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
	mapInit( );
	map.map[0][0] = TILE_BOX;
	mapDraw( );
	getch( );
	endwin( );
	return 0;
}
