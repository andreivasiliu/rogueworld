/* Periodic events, such as movement. */


#include "common.h"


void move_players( )
{
   PLAYER *pl;
   int x, y;
   
   for ( pl = players; pl; pl = pl->next )
     {
	if ( pl->cursor_x == pl->pos_x &&
	     pl->cursor_y == pl->pos_y )
	  continue;
	
	if ( pl->cursor_x < pl->pos_x ) x = -1;
	else if ( pl->cursor_x > pl->pos_x ) x = 1;
	else x = 0;
	
	if ( pl->cursor_y < pl->pos_y ) y = -1;
	else if ( pl->cursor_y > pl->pos_y ) y = 1;
	else y = 0;
	
	pl_move( pl, y, x );
     }
}



void tick_event( )
{
   move_players( );
   
   
}

