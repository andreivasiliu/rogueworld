/* Communication protocol. This will parse data from the network,
 * and format data to send to the network. */


#include <stdio.h> // must disappear
#include <ctype.h> // this too
#include <string.h> // this three
#include <unistd.h>

#include "common.h"


// This belongs in player handling, not here. Temporary.
void player_disconnected( PLAYER *player, int by_error )
{
   /* Do we care? */
   if ( !player )
     return;
   
   
}


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


void kill_connection( CONN *c, char *msg )
{
   // send kill pachet and message
   
   destroy_connection( c );
}

