/* RogueWorld Protocol parser. */


#include <stdio.h>

#include "common.h"
#include "rwp_common.h"



void parse_packet( char *packet, int size )
{
   struct packet_header header;
   int mid;
   
   if ( size < PACKET_HEADER_SIZE )
     {
	die( "Packet too short!" );
	return;
     }
   
   rwp_parse_header( packet, &header );
   
   if ( size != header.length )
     {
	die( "Packet size invalid!" );
	return;
     }
   
   mid = header.msg_id;
   
   if ( mid == MSG_USERINFO )
     {
	char *name;
	if ( !rwp_parse( packet, "z", &name ) )
	  return;
	
	update_userinfo( name );
     }
   else if ( mid == MSG_MAP )
     {
	char *map, height, width;
	
	if ( !rwp_parse( packet, "11z", &height, &width, &map ) )
	  return;
	
	update_map( height, width, map );
     }
   else if ( mid == MSG_MOVEMENT )
     {
	int obj_nr, obj_type;
	char pos_y, pos_x;
	
	if ( !rwp_parse( packet, "4411", &obj_nr, &obj_type, &pos_y, &pos_x ) )
	  return;
	
	object_movement( obj_nr, obj_type, pos_y, pos_x );
     }
   else if ( mid == MSG_DISAPPEAR )
     {
	int obj_nr;
	
	if ( !rwp_parse( packet, "4", &obj_nr ) )
	  return;
	
	object_disappeared( obj_nr );
     }
   else if ( mid == MSG_CLOSE )
     {
	char buf[8192], *msg;
	
	if ( !rwp_parse( packet, "z", &msg ) )
	  return;
	
	sprintf( buf, "Server closed the connection: '%s'", msg );
	die( buf );
     }
   else
     {
	die( "Received unknown packet type." );
     }
}


// OLD CODE - Ignore
#if 0

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
	int obj_nr, obj_type;
	char pos_y, pos_x;
	
	p = read_int32( p, &obj_nr );
	p = read_int32( p, &obj_type );
	p = read_int8( p, &pos_y );
	p = read_int8( p, &pos_x );
	
	object_movement( obj_nr, obj_type, pos_y, pos_x );
     }
   else if ( mid == MSG_DISAPPEAR )
     {
	int obj_nr;
	
	p = read_int32( p, &obj_nr );
	
	object_disappeared( obj_nr );
     }
   else if ( mid == MSG_CLOSE )
     {
	char buf2[2048];
	
	p = read_zstring( p, buf, 1024 );
	
	sprintf( buf2, "Server closed the connection: '%s'", buf );
	die( buf );
     }
}
#endif


void parse_data( char *data, int size )
{
   int packet_size;
   
   if ( size < PACKET_HEADER_SIZE )
     {
	die( "Mini-packets not allowed. This connection is going buggy..." );
     }
   
   packet_size = get_packet_length( data );
   
   if ( packet_size == 0 )
     {
	die( "Zero-sized packet? That's silly..." );
     }
   
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

