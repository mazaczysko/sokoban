#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <inttypes.h>

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

	getch( );
	endwin( );
	return 0;
}
