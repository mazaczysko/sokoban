#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <inttypes.h>

#define TILE_COUNT 1
#define TILE_AIR 0

typedef struct
{
	uint16_t id;
	char chr;
} Tile;

Tile tiles[TILE_COUNT] =
{
	{TILE_AIR, 'a' }
};

struct
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
	map.map[0][0].chr = 'x';
	mapDraw( );
	getch( );
	endwin( );
	return 0;
}
