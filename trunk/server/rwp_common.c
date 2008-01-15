/* RogueWorld Protocol common code. This will be the same for both the
 * client and the server. */


#include <stdarg.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "rwp_common.h"



int rwp_parse( char *packet, char *types, ... )
{
   char *p = packet + PACKET_HEADER_SIZE;
   char *cdest;
   int *idest;
   char **zdest;
   va_list args;
   
   va_start( args, types );
   while ( *types )
     {
	if ( *types == '1' )
	  {
	     if ( p - packet + 1 > MAX_PACKET_SIZE )
	       return 0;
	     
	     cdest = va_arg( args, char * );
	     *cdest = *(p++);
	  }
	if ( *types == '4' )
	  {
	     if ( p - packet + 4 > MAX_PACKET_SIZE )
	       return 0;
	     
	     idest = va_arg( args, int * );
	     *idest = ntohl( *(int *)p );
	     p += 4;
	  }
	if ( *types == 'z' )
	  {
	     zdest = va_arg( args, char ** );
	     *zdest = p;
	     
	     while ( *p )
	       {
		  if ( p - packet + 1 > MAX_PACKET_SIZE )
		    return 0;
		  p++;
	       }
	     if ( p - packet + 1 > MAX_PACKET_SIZE )
	       return 0;
	     p++;
	  }
	
	types++;
     }
   va_end( args );
   
   return 1;
}


int get_packet_length( char *data )
{
   struct packet_header *header;
   
   header = (struct packet_header *) data;
   
   return ntohl( header->length );
}


void rwp_parse_header( char *data, struct packet_header *dest )
{
   struct packet_header *header;
   
   header = (struct packet_header *) data;
   
   dest->length = ntohl( header->length );
   dest->msg_id = ntohl( header->msg_id );
   dest->msg_nr = ntohl( header->msg_nr );
   dest->obj_id = ntohl( header->obj_id );
   dest->obj_nr = ntohl( header->obj_nr );
}


void destroy_packet( PACKET *packet, PACKET_QUEUE *pqueue )
{
   PACKET *p;
   
   if ( packet == pqueue->packets )
     pqueue->packets = pqueue->packets->next;
   else
     for ( p = pqueue->packets; p; p = p->next )
       if ( p->next == packet )
	 {
	    p->next = packet->next;
	    break;
	 }
   
   free( packet->raw );
   free( packet );
}



void rwp_really_send_packet( PACKET *packet, PACKET_QUEUE *pqueue )
{
   PACKET *p, *p_next;
   
   /* Clear any packets of the same obj_id, and lower obj_nr. */
   if ( packet->obj_id )
     for ( p = pqueue->packets; p; p = p_next )
       {
	  p_next = p->next;
	  
	  if ( packet->obj_id == p->obj_id &&
	       packet->obj_nr >= p->obj_nr )
	    destroy_packet( packet, pqueue );
       }
   
   /* Link it. */
   packet->next = pqueue->packets;
   pqueue->packets = packet;
   
   send_to_connection( pqueue->connection, packet->raw, packet->length );
   
   // Unfortunately, until there's a way to periodically remove 'em,
   // we'll have to destroy them right after being sent.
   
   destroy_packet( packet, pqueue );
}


int get_next_object_packet_number( PACKET_QUEUE *pqueue, int obj_id )
{
   if ( !obj_id )
     return 0;
   
   return 0;
}


PACKET *rwp_forge_packet( char *buf, int type, int len, int obj_id,
			  PACKET_QUEUE *pqueue )
{
   PACKET *packet;
   struct packet_header *header;
   
   packet = calloc( 1, sizeof( PACKET ) );
   packet->raw = malloc( len );
   memcpy( packet->raw, buf, len );
   
   packet->length = len;
   packet->msg_id = type;
   packet->msg_nr = ++pqueue->packet_count;
   packet->obj_id = obj_id;
   packet->obj_nr = get_next_object_packet_number( pqueue, obj_id );
   
   header = (struct packet_header *) packet->raw;
   
   header->length = htonl( len );
   header->msg_id = htonl( type );
   header->msg_nr = htonl( packet->msg_nr );
   header->obj_id = htonl( obj_id );
   header->obj_nr = htonl( packet->obj_nr );
   
   return packet;
}


void rwp_send_packet( PACKET_QUEUE *pqueue,
		      int type, int objnr, char *types, ... )
{
   PACKET *packet;
   char buf[4096];
   char *p;
   int *intp;
   int len;
   va_list args;
   
   p = buf + PACKET_HEADER_SIZE;
   len = PACKET_HEADER_SIZE;
   
   va_start( args, types );
   while ( *types )
     {
	if ( *types == '1' )
	  {
	     len += 1;
	     if ( len >= MAX_PACKET_SIZE )
	       break;
	     
	     /* Even though it's 'char', it expects int anyway...
	      * Try 'char', and gcc will tell you why. */
	     *(p++) = va_arg( args, int );
	  }
	else if ( *types == '4' )
	  {
	     len += 4;
	     if ( len >= MAX_PACKET_SIZE )
	       break;
	     
	     /* Yeah.. pretty messy... but it's the least messy way. */
	     intp = (int *) p;
	     *(intp++) = htonl( va_arg( args, int ) );
	     p = (char *) intp;
	  }
	else if ( *types == 'z' )
	  {
	     char *zstr = va_arg( args, char * );
	     
	     while ( *zstr )
	       {
		  len += 1;
		  if ( len >= MAX_PACKET_SIZE )
		    break;
		  
		  *(p++) = *(zstr++);
	       }
	     
	     len += 1;
	     if ( len >= MAX_PACKET_SIZE )
	       break;
	     
	     *(p++) = 0;
	  }
	
	types++;
     }
   va_end( args );
   
   if ( len > MAX_PACKET_SIZE )
     return;
   
   /* Go back and write the header. */
   packet = rwp_forge_packet( buf, type, len, objnr, pqueue );
   
   rwp_really_send_packet( packet, pqueue );
}


void create_packet_queue( CONN *c )
{
   PACKET_QUEUE *pqueue;
   
   pqueue = calloc( 1, sizeof( PACKET_QUEUE ) );
   
   pqueue->connection = c;
   c->pqueue = pqueue;
}


void destroy_packet_queue( CONN *c )
{
   PACKET *p;
   
   if ( !c->pqueue )
     return;
   
   while( ( p = c->pqueue->packets ) )
     destroy_packet( p, c->pqueue );
   
   free( c->pqueue );
   c->pqueue = 0;
}



// OLD CODE - Ignore
#if 0

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


char *write_int8( char *p, char a )
{
   *p = a;
   
   return p+1;
}


char *read_int8( char *p, char *a )
{
   *a = *p;
   
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

#endif
