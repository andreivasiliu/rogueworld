/* RogueWorld Protocol parser. */


#include "common.h"
#include "rwp_common.h"


void parse_packet( char *packet, int size )
{
   int msg_size, msg_number, mid;
   char buf[1024], *p = packet;
   
   // On the switch to UDP, check size for validity.
   
   p = read_int32( p, &msg_size );
   p = read_int32( p, &msg_number );
   p = read_int32( p, &mid );
   
   if ( mid == MSG_USERINFO )
     {
	char pos_y, pos_x;
	
	p = read_zstring( p, buf, 1024 );
	p = read_int8( p, &pos_y );
	p = read_int8( p, &pos_x );
	
	update_userinfo( buf, pos_y, pos_x );
     }
   else if ( mid == MSG_MAP )
     {
	char height, width;
	
	p = read_int8( p, &height );
	p = read_int8( p, &width );
	
	update_map( height, width, p );
     }
   else if ( mid == MSG_MOVEMENT )
     {
	char pos_y, pos_x;
	
	p = read_int8( p, &pos_y );
	p = read_int8( p, &pos_x );
	
	player->pos_y = pos_y;
	player->pos_x = pos_x;
     }
}


void parse_data( char *data, int size )
{
   int packet_size;
   
   read_int32( data, &packet_size );
   
   if ( packet_size == size )
     {
	parse_packet( data, packet_size );
     }
   else if ( packet_size < size )
     {
	parse_packet( data, packet_size );
	
	parse_data( data + packet_size, size - packet_size );
     }
   else
     {
	die( "Packet stream is becoming corrupt." );
     }
}

