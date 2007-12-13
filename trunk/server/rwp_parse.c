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


void parse_pachet( CONN *c, char *pachet, int size )
{
   char buf[1024];
   
   pachet = get_word( pachet, &size, buf );
   
   if ( !strcmp( buf, "login" ) )
     {
	get_word( pachet, &size, buf );
	
	pl_login( c, buf );
     }
}


void parse_data( CONN *c, char *data, int size )
{
   printf( "Received %d bytes:\n", size );
   write( 1, data, size );
   
   parse_pachet( c, data, size );
}


