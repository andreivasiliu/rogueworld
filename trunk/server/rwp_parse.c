/* RogueWorld Communication protocol. This will parse data from the
 * network. */


#include "common.h"
#include "rwp_common.h"



void parse_packet( CONN *c, char *packet, int size )
{
   int msg_size, msg_number, mid;
   char buf[1024], *p = packet;
   PLAYER *pl = c->player;
   
   // On the switch to UDP, check size for validity.
   
   p = read_int32( p, &msg_size );
   p = read_int32( p, &msg_number );
   p = read_int32( p, &mid );
   
   //debugf( "Received packet %d (size=%d, mid=%d).", msg_number, msg_size, mid );
   
   if ( mid == MSG_LOGIN )
     {
	p = read_zstring( p, buf, 1024 );
	pl_login( c, buf );
     }
   else if ( !c->player )
     {
	debugf( "Message (%d) on non-player connection.", mid );
	return;
     }
   else if ( mid == MSG_ENTERWORLD )
     {
	pl_enterworld( pl );
     }
   else if ( mid == MSG_SETCURSOR )
     {
	char cursor_y, cursor_x;
	
	p = read_int8( p, &cursor_y );
	p = read_int8( p, &cursor_x );
	
	pl->cursor_y = cursor_y;
	pl->cursor_x = cursor_x;
     }
   else
     debugf( "Message ID (%d) unknown.", mid );
}


void parse_data( CONN *c, char *data, int size )
{
   int packet_size;
   
   read_int32( data, &packet_size );
   
   if ( packet_size == size )
     {
	parse_packet( c, data, packet_size );
	data += packet_size;
	size -= packet_size;
     }
   else
     {
	debugf( "Warning: packet pattern is becoming corrupt." );
	debugf( "Reported size: %d. Actual size: %d.", packet_size, size );
     }
}


