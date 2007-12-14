/* RogueWorld Communication protocol. This will format data into packets and
 * send it to the network. */


#include "common.h"



void kill_connection( CONN *c, char *msg )
{
   // send kill packet and message
   
   destroy_connection( c );
}


