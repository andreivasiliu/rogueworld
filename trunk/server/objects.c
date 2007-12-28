/* Object handling. */


#include "common.h"


#define SQUARE(x) ((x)*(x))


int can_see_spot( OBJECT *obj, int pos_y, int pos_x )
{
   int dist2;
   
   /* For now, just check the distance, not the actual line of sight. */
   
   /* dist^2 = (x1 - x2)^2 + (y1 - y2)^2 */
   dist2 = SQUARE( obj->pos_x - pos_x ) + SQUARE( obj->pos_y - pos_y );
   
   /* For now, lets say that the line of sight extends up to 10 squares. */
   if ( dist2 <= 100 )
     return 1;
   else
     return 0;
}



void move_object( OBJECT *obj, short y, short x )
{
   PLAYER *pl;
   short pos_y, pos_x;
   
   pos_y = obj->pos_y + y;
   pos_x = obj->pos_x + x;
   
   if ( map.map[pos_y*map.width+pos_x] == '#' )
     return;
   
   /* Send movement packets to players. */
   for ( pl = players; pl; pl = pl->next )
     {
	if ( can_see_spot( pl->persona, obj->pos_y, obj->pos_x ) ||
	     can_see_spot( pl->persona, pos_y, pos_x ) )
	  send_movement( pl, obj, pos_y, pos_x );
     }
   
   obj->pos_y = pos_y;
   obj->pos_x = pos_x;
}

