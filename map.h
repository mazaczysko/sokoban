#ifndef MAP_H
#define MAP_H
#include <inttypes.h>

#define TILE_AIR_ID 0
#define TILE_PLAYER_ID 1
#define TILE_SOCKET_ID 2
#define TILE_BOX_ID 3
#define TILE_SOCKETPLAYER_ID 4
#define TILE_SOCKETBOX_ID 5
#define TILE_WALL_ID 6

typedef struct
{
	uint16_t id;
	uint8_t fg, bg;
	char chr;

} Tile;

typedef struct
{
	uint16_t width, height;
	Tile **map;
} Map;

extern Map map;
extern void mapInit( );
extern void mapDraw( );
extern uint8_t mapLoad( const char *fname );
extern void getPlayerPos( uint16_t *x, uint16_t *y );
extern int tileMove( uint16_t x, uint16_t y, int8_t dx, int8_t dy );

#endif
