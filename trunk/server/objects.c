/* Object handling. */


#include <stdlib.h>

#include "common.h"


#define SQUARE(x) ((x)*(x))


OBJECT *objects;
int highest_vnum = 0;


OBJECT *create_object( int type )
{
   OBJECT *obj;
   
   obj = calloc( 1, sizeof( OBJECT ) );
   
   obj->next = objects;
   objects = obj;
   
   obj->vnum = ++highest_vnum;
   obj->type = type;
   
   return obj;
}


void destroy_object( OBJECT *obj )
{
   OBJECT *prev;
   
   displace_object( obj );
   
   /* Unlink */
   if ( objects == obj )
     objects = objects->next;
   else
     for ( prev = objects; prev->next; prev = prev->next )
       if ( prev->next == obj )
	 {
	    prev->next = obj->next;
	    break;
	 }
   
   free( obj );
}


OBJECT *create_persona( short pos_y, short pos_x )
{
   OBJECT *obj;
   
   obj = create_object( OBJ_PLAYER );
   place_object( obj, pos_y, pos_x );
   
   return obj;
}


int spot_can_see_spot( int pos1_y, int pos1_x, int pos2_y, int pos2_x )
{
   int dist2;
   
   /* For now, just check the distance, not the actual line of sight. */
   
   /* dist^2 = (x1 - x2)^2 + (y1 - y2)^2 */
   dist2 = SQUARE( pos1_y - pos2_y ) + SQUARE( pos1_x - pos2_x );
   
   /* For now, lets say that the line of sight extends up to 10 squares. */
   if ( dist2 <= 100 )
     return 1;
   else
     return 0;
}


int can_see_spot( OBJECT *obj, int pos_y, int pos_x )
{
   return spot_can_see_spot( obj->pos_y, obj->pos_x,
			     pos_y, pos_x );
}


int can_see( OBJECT *obj1, OBJECT *obj2 )
{
   return spot_can_see_spot( obj1->pos_y, obj1->pos_x,
			     obj2->pos_y, obj2->pos_x );
}


void place_object( OBJECT *obj, short pos_y, short pos_x )
{
   PLAYER *pl;
   
   obj->pos_y = pos_y;
   obj->pos_x = pos_x;
   
   /* Notify others. */
   for ( pl = players; pl; pl = pl->next )
     if ( pl->persona != obj )
       if ( can_see( pl->persona, obj ) )
	 send_movement( pl, obj, pos_y, pos_x );
   
   /* Notify itself of others (and itself). */
   pl = obj->player;
   if ( !pl )
     return;
   
   for ( obj = objects; obj; obj = obj->next )
     if ( can_see( pl->persona, obj ) )
       send_movement( pl, obj, obj->pos_y, obj->pos_x );
}


void displace_object( OBJECT *obj )
{
   PLAYER *pl;
   
   for ( pl = players; pl; pl = pl->next )
     if ( can_see( pl->persona, obj ) )
       send_disappear( pl, obj );
}


void move_object( OBJECT *obj, short y, short x )
{
   PLAYER *pl;
   OBJECT *o;
   short pos_y, pos_x;
   
   pos_y = obj->pos_y + y;
   pos_x = obj->pos_x + x;
   
   if ( map.map[pos_y*map.width+pos_x] == '#' )
     return;
   
   /* Send movement packets to players. */
   for ( pl = players; pl; pl = pl->next )
     {
	if ( obj == pl->persona )
	  continue;
	
	/* FIXME: This is too counter-intuitive. Find another way. */
	
	/* Saw something move. */
	if ( can_see_spot( pl->persona, pos_y, pos_x ) )
	  send_movement( pl, obj, pos_y, pos_x );
	/* Saw something move out of sight. */
	else if ( can_see_spot( pl->persona, obj->pos_y, obj->pos_x ) )
	  send_disappear( pl, obj );
     }
   
   pl = obj->player;
   if ( pl )
     {
	/* Send movement about objects entering/leaving the line of sight. */
	for ( o = objects; o; o = o->next )
	  if ( o != pl->persona )
	    {
	       /* Couldn't see, can see now. */
	       if ( !can_see( pl->persona, o ) &&
		    spot_can_see_spot( pos_y, pos_x, o->pos_y, o->pos_x ) )
		 {
		    send_movement( pl, o, o->pos_y, o->pos_x );
		 }
	       
	       /* Could see, can't see now. */
	       if ( can_see( pl->persona, o ) &&
		    !spot_can_see_spot( pos_y, pos_x, o->pos_y, o->pos_x ) )
		 {
		    send_disappear( pl, o );
		 }
	    }
	
	/* Send movement about itself. */
	send_movement( pl, obj, pos_y, pos_x );
     }
   
   obj->pos_y = pos_y;
   obj->pos_x = pos_x;
}

