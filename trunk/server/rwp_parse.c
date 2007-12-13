/* RogueWorld Communication protocol. This will parse data from the
 * network. */


#include <stdio.h> // must disappear
#include <ctype.h> // this too
#include <string.h> // this three
#include <unistd.h>

#include "common.h"


// Temporary.
char *get_word( char *src, int *src_length, char *dest )
{
   while ( isspace( *src ) && *src_length > 0 )
     src++, (*src_length)--;
   
   while ( !isspace( *src ) && *src_length > 0 )
     *(dest++) = *(src++), (*src_length)--;
   
   return src;
}


void parse_packet( CONN *c, char *packet, int size )
{
   char buf[1024];
   
   packet = get_word( packet, &size, buf );
   
   if ( !strcmp( buf, "login" ) )
     {
	get_word( packet, &size, buf );
	
	pl_login( c, buf );
     }
}


void parse_data( CONN *c, char *data, int size )
{
   printf( "Received %d bytes:\n", size );
   write( 1, data, size );
   
   parse_packet( c, data, size );
}


