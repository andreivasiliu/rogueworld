/* This will perform updates received from the server, such as player
 * position, objects, etc.
 */


#include <stdlib.h>
#include <string.h>
#include <curses.h>

#include "common.h"


void update_userinfo( char *name, int pos_y, int pos_x )
{
   if ( !player )
     player = calloc( 1, sizeof( PLAYER ) );
   
   if ( player->name )
     free( player->name );
   
   player->name = strdup( name );
   
   mvaddstr( 8, 2, "Received userinfo." );
   refresh( );
}


void update_map( int height, int width, char *map_str )
{
   if ( !map )
     map = calloc( 1, sizeof( MAP ) );
   
   if ( map->map )
     free( map->map );
   
   map->map = strdup( map_str );
   map->height = height;
   map->width = width;
   
   mvaddstr( 9, 2, "Received map." );
   refresh( );
}

