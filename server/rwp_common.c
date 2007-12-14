/* RogueWorld Protocol common code. This will be the same for both the
 * client and the server. */


#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "rwp_common.h"


#define PACKET_HEADER_SIZE	12
#define MAX_PACKET_SIZE		4096



char *write_zstring( char *p, char *str )
{
   int len;
   
   len = strlen( str );
   memcpy( p, str, len + 1 );
   
   return p + len + 1;
}


char *read_zstring( char *p, char *dest, int max )
{
   while ( *p && max > 1 )
     *(dest++) = *(p++), max--;
   
   *dest = 0;
   
   return p+1;
}


char *write_int32( char *p, int n )
{
   int *np = (int *)p;
   
   *np = htonl( n );
   np++;
   
   return (char *)np;
}


char *read_int32( char *p, int *dest )
{
   *dest = ntohl( *(int*)p );
   
   return p + 4;
}


char *new_packet( int message_id, int packet_size )
{
   char *packet;
   char *p;
   
   packet_size += PACKET_HEADER_SIZE;
   
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


void send_packet( CONN *c, char *packet )
{
   /* Take the length as the first int32 of the packet. */
   send_to_client( c, packet, ntohl( *(int *)packet ) );
   
   free( packet );
}


