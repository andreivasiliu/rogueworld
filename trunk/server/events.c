/* Periodic events, such as movement. */


#include "common.h"


void move_players( )
{
   PLAYER *pl;
   OBJECT *persona;
   int x, y;
   
   for ( pl = players; pl; pl = pl->next )
     {
	persona = pl->persona;
	if ( pl->cursor_x == persona->pos_x &&
	     pl->cursor_y == persona->pos_y )
	  continue;
	
	if ( pl->cursor_x < persona->pos_x ) x = -1;
	else if ( pl->cursor_x > persona->pos_x ) x = 1;
	else x = 0;
	
	if ( pl->cursor_y < persona->pos_y ) y = -1;
	else if ( pl->cursor_y > persona->pos_y ) y = 1;
	else y = 0;
	
	move_object( pl->persona, y, x );
     }
}



void tick_event( )
{
   move_players( );
   
   
}

