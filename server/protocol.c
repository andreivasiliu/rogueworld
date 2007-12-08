/* Communication protocol. This will parse data from the network,
 * and format data to send to the network. */


#include <stdio.h> // must disappear
#include <unistd.h>

#include "common.h"


// This belongs in player handling, not here. Temporary.
void player_disconnected( PLAYER *player, int by_error )
{
   /* Do we care? */
   if ( !player )
     return;
   
   
}


void parse_data( CONN *c, char *data, int size )
{
   printf( "Received %d bytes:\n", size );
   write( 1, data, size );
   
   
   
}

