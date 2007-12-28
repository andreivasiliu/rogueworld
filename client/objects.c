/* Objects on the map. */


#include <stdlib.h>

#include "common.h"


OBJECT *objects;


OBJECT *create_object( int obj_nr, int obj_type )
{
   OBJECT *obj;
   
   obj = calloc( 1, sizeof( OBJECT ) );
   
   obj->type = obj_type;
   obj->vnum = obj_nr;
   
   /* Unlinking is fun, but link? It gets mind-blowing.. */
   if ( objects )
     objects->prev = &obj->next;
   obj->next = objects;
   obj->prev = &objects;
   objects = obj;
   
   return obj;
}


void destroy_object( OBJECT *obj )
{
   /* Unlink. */
   obj->next->prev = obj->prev;
   *obj->prev = obj->next;
   
   /* Free. */
   free( obj );
}


OBJECT *get_object( int obj_nr )
{
   OBJECT *obj;
   
   for ( obj = objects; obj; obj = obj->next )
     if ( obj->vnum == obj_nr )
       return obj;
   
   return NULL;
}


void object_movement( int obj_nr, int obj_type, short pos_y, short pos_x )
{
   OBJECT *obj;
   
   obj = get_object( obj_nr );
   
   /* New object? */
   if ( !obj )
     obj = create_object( obj_nr, obj_type );
   
   obj->pos_y = pos_y;
   obj->pos_x = pos_x;
   
   /* You, as in, yes, YOU. */
   if ( obj_type == OBJ_YOU )
     if ( player->body != obj )
       {
	  player->body = obj;
	  cursor_y = obj->pos_y;
	  cursor_x = obj->pos_x;
       }
}


void object_disappeared( int obj_nr )
{
   OBJECT *obj;
   
   obj = get_object( obj_nr );
   
   if ( obj )
     destroy_object( obj );
}

