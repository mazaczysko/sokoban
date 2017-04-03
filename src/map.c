#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <inttypes.h>
#include <ctype.h>
#include "map.h"

Map map;

//								ID						FG	BG	CHAR
const Tile TILE_AIR = 			{TILE_AIR_ID,			0,	0,	' ' };
const Tile TILE_PLAYER = 		{TILE_PLAYER_ID,		7,	0,	'@' };
const Tile TILE_SOCKET = 		{TILE_SOCKET_ID,		0,	4,	' '	};
const Tile TILE_BOX = 			{TILE_BOX_ID,			2,	0,	'#' };
const Tile TILE_SOCKETPLAYER = 	{TILE_SOCKETPLAYER_ID,	7,	4,	'@' };
const Tile TILE_SOCKETBOX = 	{TILE_SOCKETBOX_ID,		3,	5,	'#'	};
const Tile TILE_WALL = 			{TILE_WALL_ID,			0,	6,	' ' };

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
		{
			attron( COLOR_PAIR( map.map[j][i].fg * 8 + map.map[j][i].bg ) );
			mvprintw( i, j, "%c", map.map[j][i].chr );
			attroff( COLOR_PAIR( map.map[j][i].fg * 8 + map.map[j][i].bg ) );
		}

	}

}

uint8_t mapLoad( const char *fname )
{
	int c;
	uint16_t x = 0, y = 0, pcnt = 0;
	Tile t;
	FILE *f;

	//Open file
	if ( fname == NULL || ( f = fopen( fname, "r" ) ) == NULL )
		return MAP_ERR_FILE;

	//Read character by character
	while ( ( c = getc( f ) ) != EOF )
	{
		//Check level size, if bigger than terminal - abort
		if (x >= map.width || y >= map.height )
		{
			return MAP_ERR_SIZE;
		}
		//Switching characters into tiles
		switch ( ( char ) c )
		{
			case '\r':
			case '\n':
				x = 0;
				y++;
				continue;
				break;

			case '@':
					t = TILE_PLAYER;
					pcnt++;
					break;

			case 'w':
					t = TILE_WALL;
					break;

			case '.':
					t = TILE_SOCKET;
					break;

			case '&':
					t = TILE_BOX;
					break;

			case 'P':
					t = TILE_SOCKETPLAYER;
					pcnt++;
					break;

			case 'B':
					t = TILE_SOCKETBOX;
					break;
			//e.g. space
			default:
				t = TILE_AIR;
				break;

		}

		//writing tiles into map
		map.map[x++][y] = t;
	}

	//Check player count, if different from 1, abort
	if( pcnt != 1 )
	{
		return MAP_ERR_PCNT;
	}

	//End reading
	fclose( f );
	return MAP_OK;
}

void getPlayerPos( uint16_t *x, uint16_t *y )
{
	uint16_t i, j;

	//Find player
	if ( y == NULL || x == NULL ) return;
	for ( i = 0; i < map.height; i++ )
		for ( j = 0; j < map.width; j++ )
			if ( map.map[j][i].id == TILE_SOCKETPLAYER_ID || map.map[j][i].id == TILE_PLAYER_ID )
			{
				*x = j;
				*y = i;
				return;
			}
}

int tileMove( uint16_t x, uint16_t y, int8_t dx, int8_t dy )
{
	Tile *obj, *dest;

	//Deltas limit
	if ( dx ) dx = dx < 0 ? -1 : 1;
	if ( dy ) dy = dy < 0 ? -1 : 1 ;

	//Check terminal borders
	if( x < 0 || y < 0 || x >= map.width || y >= map.height ) return 0;
	if( x + dx < 0 || y + dy < 0 || x + dx >= map.width || y + dy >= map.height ) return 0;

	//Get pointers for convenience
	obj = &map.map[x][y];
	dest = &map.map[x + dx][y + dy];

	//Returned valuse
	//0 - unable to move
	//1 - succesfully moved

	switch( obj->id )
	{
		//Player or player on socket
		case TILE_SOCKETPLAYER_ID:
		case TILE_PLAYER_ID:
			switch ( dest->id )
			{
				//Always can be moved to empty field
				case TILE_AIR_ID:
					*obj = ( obj->id == TILE_PLAYER_ID ? TILE_AIR : TILE_SOCKET );
					*dest = TILE_PLAYER;
					return 1;
					break;
				//Player can push box if box can be moved
				case TILE_BOX_ID:
					if ( tileMove( x + dx, y + dy, dx, dy ) )
					{
						*obj = ( obj->id == TILE_PLAYER_ID ? TILE_AIR : TILE_SOCKET );
						*dest = TILE_PLAYER;
						return 1;
					}
					return 0;
					break;

				//Player always can stand on another socket
				case TILE_SOCKET_ID:
					*obj = ( obj->id == TILE_PLAYER_ID ? TILE_AIR : TILE_SOCKET );
					*dest = TILE_SOCKETPLAYER;
					return 1;
					break;

				//Player can push box from full socket if box can be moved
				case TILE_SOCKETBOX_ID:
					if ( tileMove( x + dx, y + dy, dx, dy ) )
					{
						*obj = ( obj->id == TILE_PLAYER_ID ? TILE_AIR : TILE_SOCKET );
						*dest = TILE_SOCKETPLAYER;
						return 1;
					}
					return 0;
					break;

				//in case of other cases (e.g. walls)
				default:
				 	return 0;
					break;

			}
		break;

		//Box or box on socket
		case TILE_BOX_ID:
		case TILE_SOCKETBOX_ID:
			switch ( dest->id )
			{
				//Box always can be moved onto empty field
				case TILE_AIR_ID:
					*obj = ( obj->id == TILE_BOX_ID ? TILE_AIR : TILE_SOCKETBOX);
					*dest = TILE_BOX;
					return 1;
					break;

				//Box can be always moved onto socket
				case TILE_SOCKET_ID:
					*obj = ( obj->id == TILE_BOX_ID ?TILE_AIR : TILE_SOCKETBOX);
					*dest = TILE_SOCKETBOX;
					return 1;
					break;

				default:
					return 0;
					break;

			}
		break;
	}

	return 0;
}

uint8_t checkWinner( )
{
	uint16_t i, j, cnt = 0;

		for( i = 0; i < map.width; i++ )
			for( j = 0; j < map.height; j++ )
				cnt += map.map[i][j].id == TILE_BOX_ID;
	return !cnt;
}
