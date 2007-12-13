/* RogueWorld Protocol packet maker. */


#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"


#define PACKET_HEADER_SIZE	12
#define MAX_PACKET_SIZE		4096

#define MSG_LOGIN	1


char *write_zstring( char *p, char *str )
{
   int len;
   
   len = strlen( str );
   memcpy( p, str, len + 1 );
   
   return p + len + 1;
}


char *write_int32( char *p, int n )
{
   int *np = (int *)p;
   
   *np = htonl( n );
   np++;
   
   return (char *)np;
}


char *new_packet( int packet_size, int message_id )
{
   char *packet;
   char *p;
   
   if ( packet_size > MAX_PACKET_SIZE )
     return 0;
   
   packet = calloc( 1, packet_size );
   p = packet;
   
   p = write_int32( p, packet_size );
   p = write_int32( p, 0 );
   p = write_int32( p, message_id );
   
   return packet;
}



char *skip_header( char *packet )
{
   return packet + PACKET_HEADER_SIZE;
}


void send_packet( char *packet )
{
   send_to_server( packet, *(int *)packet );
}


void send_login( char *name )
{
   char *packet;
   char *p;
   int packet_size;
   
   packet_size = PACKET_HEADER_SIZE + strlen( name ) + 1;
   
   packet = new_packet( packet_size, MSG_LOGIN );
   if ( !packet )
     return;
   
   p = skip_header( packet );
   p = write_zstring( p, name );
   
   send_packet( packet );
}

