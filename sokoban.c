#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <inttypes.h>

#define TILE_COUNT 7
#define TILE_AIR 0
#define TILE_PLAYER 1
#define TILE_SOCKET 2
#define TILE_BOX 3
#define TILE_SOCKETPLAYER 4
#define TILE_SOCKETBOX 5
#define TILE_WALL 6
typedef struct
{
	uint16_t id;
	uint8_t fg, bg;
	char chr;

} Tile;

Tile tiles[TILE_COUNT] =
{
	{TILE_AIR, 			0, 	0, 	' ' },
	{TILE_PLAYER, 		7, 	0, 	'@' },
	{TILE_SOCKET,		0, 	4, 	' '	},
	{TILE_BOX, 			2, 	0, 	'#' },
	{TILE_SOCKETPLAYER,	7,	4,	'@' },
	{TILE_SOCKETBOX,	3,	5, 	'#'	},
	{TILE_WALL,			0,	6,	' ' }

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
	map.map[0][0] = tiles[TILE_BOX];
	mapDraw( );
	getch( );
	endwin( );
	return 0;
}
