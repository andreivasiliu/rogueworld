/* RogueWorld Protocol packet maker. */


#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "rwp_common.h"



void send_login( char *name )
{
   rwp_send_packet( server->pqueue, MSG_LOGIN, 0,
		    "z", name );
}


void send_enterworld( )
{
   rwp_send_packet( server->pqueue, MSG_ENTERWORLD, 0,
		    "" );
}


void send_setcursor( int y, int x )
{
   rwp_send_packet( server->pqueue, MSG_SETCURSOR, 0,
		    "11", y, x );
}

void send_close( char *reason )
{
   rwp_send_packet( server->pqueue, MSG_CLOSE, 0,
		    "z", reason );
}


// OLD CODE - Ignore
#if 0

void send_packet( char *packet )
{
   /* Take the length as the first int32 of the packet. */
   send_to_server( packet, ntohl( *(int *)packet ) );
   
   free( packet );
}



void send_login( char *name )
{
   char *packet, *p;
   
   packet = new_packet( MSG_LOGIN, strlen( name ) + 1 );
   if ( !packet )
     return;
   
   p = skip_header( packet );
   p = write_zstring( p, name );
   
   send_packet( packet );
}


void send_enterworld( )
{
   char *packet;
   
   packet = new_packet( MSG_ENTERWORLD, 0 );
   if ( !packet )
     return;
   
   send_packet( packet );
}


void send_setcursor( int y, int x )
{
   char *packet, *p;
   
   packet = new_packet( MSG_SETCURSOR, 2*sizeof(char) );
   if ( !packet )
     return;
   
   p = skip_header( packet );
   p = write_int8( p, y );
   p = write_int8( p, x );
   
   send_packet( packet );
}

void send_close( char *reason )
{
   char *packet, *p;
   
   packet = new_packet( MSG_CLOSE, strlen( reason ) + 1 );
   if ( !packet )
     return;
   
   p = skip_header( packet );
   p = write_zstring( p, reason );
   
   send_packet( packet );
}

#endif
