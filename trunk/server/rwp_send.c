/* RogueWorld Communication protocol. This will format data into packets and
 * send it to the network. */


#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#include "common.h"
#include "rwp_common.h"



void send_packet( CONN *c, char *packet )
{
   /* Take the length as the first int32 of the packet. */
   send_to_client( c, packet, ntohl( *(int *)packet ) );
   
   free( packet );
}



void kill_connection( CONN *c, char *msg )
{
   // send kill packet and message
   
   destroy_connection( c );
}


void send_map( CONN *c, MAP *map )
{
   char *packet, *p;
   
   packet = new_packet( MSG_MAP, 2*sizeof(char) + strlen( map->map ) + 1 );
   if ( !packet )
     return;
   
   p = skip_header( packet );
   p = write_int8( p, map->height );
   p = write_int8( p, map->width );
   p = write_zstring( p, map->map );
   
   send_packet( c, packet );
}


void send_userinfo( CONN *c, PLAYER *player )
{
   char *packet, *p;
   
   packet = new_packet( MSG_USERINFO, strlen(player->name)+1 + 2*sizeof(char) );
   if ( !packet )
     return;
   
   p = skip_header( packet );
   p = write_zstring( p, player->name );
   
   send_packet( c, packet );
}


void send_movement( PLAYER *pl, OBJECT *obj, short pos_y, short pos_x )
{
   char *packet, *p;
   int type;
   
   packet = new_packet( MSG_MOVEMENT, 2*sizeof(char) + 2*sizeof(int) );
   if ( !packet )
     return;
   
   if ( pl->persona == obj )
     type = OBJ_YOU;
   else
     type = obj->type;
   
   p = skip_header( packet );
   p = write_int32( p, obj->vnum );
   p = write_int32( p, type );
   p = write_int8( p, pos_y );
   p = write_int8( p, pos_x );
   
   send_packet( pl->connection, packet );
}

void send_disappear( PLAYER *pl, OBJECT *obj )
{
   char *packet, *p;
   
   packet = new_packet( MSG_DISAPPEAR, sizeof(int) );
   if ( !packet )
     return;
   
   p = skip_header( packet );
   p = write_int32( p, obj->vnum );
   
   send_packet( pl->connection, packet );
}

